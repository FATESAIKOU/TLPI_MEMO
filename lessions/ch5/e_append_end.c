#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    off_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("%s pathname offset\n", argv[0]);
    }

    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("failed to open file");
    }

    off = atoll(argv[2]);
    if (lseek(fd, off, SEEK_SET) == -1) {
        printf("failed to seek file");
    }

    if (write(fd, "test", 4) == -1) {
        printf("failed to write \"test\" to file");
    }

    close(fd);

    return 0;
}
