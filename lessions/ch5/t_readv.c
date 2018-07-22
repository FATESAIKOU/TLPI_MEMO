#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdarg.h>

void ErrExit(const char* format, ...);

int main(int argc, char *argv[])
{
    // Var define
    int fd;
    struct iovec iov[3];
    struct stat myStruct; // Buffer 1
    int x;                // Buffer 2
#define STR_SIZE 100
    char str[STR_SIZE];   // Buffer 3
    ssize_t num_read, total_required;

    // Usage check
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        ErrExit("Error usage");

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        ErrExit("open");

    // Initialize & vector build
    total_required = 0;
    
    iov[0].iov_base = &myStruct;
    iov[0].iov_len = sizeof(struct stat);
    total_required += iov[0].iov_len;

    iov[1].iov_base = &x;
    iov[1].iov_len = sizeof(x);
    total_required += iov[1].iov_len;

    iov[2].iov_base = str;
    iov[2].iov_len = STR_SIZE;
    total_required += STR_SIZE;

    // Readv
    num_read = readv(fd, iov, 3);
    if (num_read == -1)
        ErrExit("readv");

    if (num_read < total_required)
        printf("Read fewer bytes than requested\n");

    // Report
    printf("total bytes requested: %ld; bytes read: %ld\n",
            (long) total_required, (long) num_read);

    exit(EXIT_SUCCESS);
}

void ErrExit(const char* format, ...)
{
    printf("Error: ");
    
    va_list marker;
    va_start(marker, format);
    vprintf(format, marker);
    va_end(marker);

    exit(-1);
}
