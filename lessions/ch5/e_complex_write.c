#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd1, fd2, fd3;

    fd1 =open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    fd2 = dup(fd1);
    fd3 = open(argv[1], O_RDWR);
    write(fd1, "Hello,", 6);
    write(fd2, " world", 6);
    lseek(fd2, 0, SEEK_SET);
    write(fd3, "Gidday", 6);
    write(fd1, "HELLO,", 6);

    close(fd1);
    close(fd2);
    close(fd3);

    return 0;
}
