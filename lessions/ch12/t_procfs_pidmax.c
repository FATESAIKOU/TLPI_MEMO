#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define MAX_LINE 100

int
main(int argc, char *argv[])
{
    int fd;
    char line [MAX_LINE];
    ssize_t n;

    fd = open("/proc/sys/kernel/pid_max", (argc > 1) ? O_RDWR : O_RDONLY);
    if (fd == -1) {
        printf("open\n");
        exit(-1);
    }

    n = read(fd, line, MAX_LINE);
    if (n == -1) {
        printf("read\n");
        exit(-1);
    }

    if (argc > 1)
        printf("Old value: ");
    printf("%.*s", (int) n, line);

    if (argc > 1) {
        if (lseek(fd, 0, SEEK_SET) == -1) {
            printf("lseek\n");
            exit(-1);
        }

        if (write(fd, argv[1], strlen(argv[1])) != strlen(argv[1])) {
            printf("fatal: write\n");
            exit(-1);
        }

        system("echo /proc/sys/kernel/pid_max now contains "
               "`cat /proc/sys/kernel/pid_max`");
    }

    exit(0);
}
