#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

void ErrExit(const char* format, ...);

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    int des_fd = open("dest.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1 || des_fd == -1)
        ErrExit("open\n");

    char buf[1];

    ssize_t num = 0;
    off_t now_off = 0;

    do {
        num = pread(fd, buf, sizeof(buf), now_off);
        now_off += num;
        printf("%s", buf);
        pwrite(des_fd, buf, sizeof(buf), now_off);
    } while(num != 0);
    
    return 0;
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
