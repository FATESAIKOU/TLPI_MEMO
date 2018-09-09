#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdarg.h>

void ErrExit(const char* format, ...);

void MkIovec(struct iovec **iovec_arr, int *iovcnt);
void MyReadvTest(char *src_path, const struct iovec *iovec_arr, int iovcnt);
void MyWritevTest(char *dest_path, const struct iovec *iovec_arr, int iovcnt);

ssize_t MyReadv(int filedes, const struct iovec *iovec_arr, int iovcnt);
ssize_t MyWritev(int filedes, const struct iovec *iovec_arr, int iovcnt);

int main(int argc, char *argv[])
{
    // Usage check
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        ErrExit("Error usage");

    struct iovec *iovec_arr;
    int iovcnt;

    // Make iovec array
    MkIovec(&iovec_arr, &iovcnt);

    // Do readv test
    MyReadvTest(argv[1], iovec_arr, iovcnt);

    // Do writev test
    MyWritevTest(argv[2], iovec_arr, iovcnt);

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

void MkIovec(struct iovec **iovec_arr, int *iovcnt)
{
    struct iovec *tmp_arr = (struct iovec *) malloc(sizeof(struct iovec) * 3);
    
    tmp_arr[0].iov_base = (void *) malloc(sizeof(struct stat));
    tmp_arr[0].iov_len = sizeof(struct stat);

    tmp_arr[1].iov_base = (void *) malloc(sizeof(int));
    tmp_arr[1].iov_len = sizeof(int);

#define STR_SIZE 100
    tmp_arr[2].iov_base = (void *) malloc(sizeof(char) * STR_SIZE);
    tmp_arr[2].iov_len = sizeof(char) * STR_SIZE;

    *iovec_arr = tmp_arr;
    *iovcnt = 3;
}

void MyReadvTest(char *src_path, const struct iovec *iovec_arr, int iovcnt)
{
    // Var define
    int fd;
    ssize_t num_read, total_required;

    fd = open(src_path, O_RDONLY);
    if (fd == -1)
        ErrExit("open");

    // Initialize & vector build
    int i;
    for (total_required = 0, i = 0; i < iovcnt; ++ i) {
        total_required += iovec_arr[i].iov_len;
    }

    // Readv
    num_read = MyReadv(fd, iovec_arr, iovcnt);
    if (num_read == -1)
        ErrExit("readv");

    if (num_read < total_required)
        printf("Read fewer bytes than requested\n");

    // Report
    printf("total bytes requested: %ld; bytes read: %ld\n",
            (long int) total_required, (long int) num_read);
}

void MyWritevTest(char *dest_path, const struct iovec *iovec_arr, int iovcnt)
{
    // Var define and openfile
    int fd;
    ssize_t num_wrote, total_required;
    
    // Initialize
    fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR |
                                                        S_IRGRP | S_IWGRP |
                                                        S_IROTH | S_IWOTH);
    int i;
    for (total_required = 0, i = 0; i < iovcnt; ++ i) {
        total_required += iovec_arr[i].iov_len;
    }

    // Writev
    num_wrote = MyWritev(fd, iovec_arr, iovcnt);
    if (num_wrote == -1)
        ErrExit("writev");

    if (num_wrote < total_required)
        printf("write fewer bytes than requested\n");
    
    // Report
    printf("total bytes requested: %ld; bytes wrote: %ld\n",
            (long int) total_required, (long int) num_wrote);
}

ssize_t MyReadv(int filedes, const struct iovec *iovec_arr, int iovcnt)
{
    int total_read, i;
    for (total_read = 0, i = 0; i < iovcnt; ++ i) {
        total_read += read(filedes, iovec_arr[i].iov_base, iovec_arr[i].iov_len);
    }

    return total_read;
}

ssize_t MyWritev(int filedes, const struct iovec *iovec_arr, int iovcnt)
{
    int total_wrote, i;
    for (total_wrote = 0, i = 0; i < iovcnt; ++ i) {
        total_wrote += write(filedes, iovec_arr[i].iov_base, iovec_arr[i].iov_len);
    }

    return total_wrote;
}
