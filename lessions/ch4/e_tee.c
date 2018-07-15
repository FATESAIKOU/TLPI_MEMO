#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int GetFd(int argc, char *argv[]);

void ErrExit(const char* format, ...);

int main(int argc, char *argv[])
{
    int new_fd = GetFd(argc, argv);
    if (new_fd == -1) {
        ErrExit("Open file failed");
    }

    char buf[1024];
    ssize_t num_read;
    while ((num_read = read(STDIN_FILENO, buf, 1024)) != 0) {
        write(new_fd, buf, num_read);
        write(STDOUT_FILENO, buf, num_read);
    }
    close(new_fd);

    return 0;
}

int GetFd(int argc, char *argv[])
{
    char c;
    int flags = O_CREAT | O_WRONLY | O_TRUNC;
    extern char *optarg;
    extern int optind, optopt;

    while ((c=getopt(argc, argv, "a")) != -1) {
        switch (c) {
            case 'a':
                flags = O_CREAT | O_WRONLY | O_APPEND;
                break;
            case '?':
                printf("Unsupport command!\n");
                return -1;
        }
    }

    int t_fd = open(argv[optind], flags, S_IRUSR | S_IWUSR);
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
