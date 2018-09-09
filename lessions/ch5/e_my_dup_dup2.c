#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GetOffset(fd) (long long)lseek(fd, 0, SEEK_CUR)

int MyDup(int fd)
{
    return fcntl(fd, F_DUPFD, 0);
}

int MyDup2(int fd1, int fd2)
{
    if (fd1 == fd2) {
        return fd2;
    }

    close(fd2);
    return fcntl(fd1, F_DUPFD, fd2);
}

int main(int argc, char *argv[])
{
    int fd1, fd2, fd3;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s pathname\n", argv[0]);
    }
    
    // Do dup
    fd1 = open(argv[1], O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
    fd2 = MyDup(fd1);
    fd3 = MyDup2(fd1, 0);
    if (fd1 == -1 || fd2 == -1) {
        printf("failed to open file");
    }

    // Flags check
    int fd1_flags = fcntl(fd1, F_GETFL);
    int fd2_flags = fcntl(fd2, F_GETFL);
    int fd3_flags = fcntl(fd3, F_GETFL);
    printf("[FLAG_CKECK]: fd1_flags: %d, dup: %d, dup2: %d\n", fd1_flags, fd2_flags, fd3_flags);

    // Do seek check
    if (lseek(fd1, 1, SEEK_SET) == -1) {
        printf("failed to seek file");
    }
    printf("[SEEK_CKECK]: fd1_offset: %lld, dup_offset: %lld, dup2_offset: %lld\n", GetOffset(fd1), GetOffset(fd2), GetOffset(fd3));

    // Do write check
    if (write(fd1, "test", 4) == -1) {
        printf("failed to write \"test\" to file");
    }
    printf("[WRITE_CKECK]: fd1_offset: %lld, dup_offset: %lld, dup2_offset: %lld\n", GetOffset(fd1), GetOffset(fd2), GetOffset(fd3));

    close(fd1);
    close(fd2);
    close(fd3);

    return 0;
}
