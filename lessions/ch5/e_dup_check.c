#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GetOffset(fd) (long long)lseek(fd, 0, SEEK_CUR)

int main(int argc, char *argv[])
{
    int fd1, fd2;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s pathname\n", argv[0]);
    }
    
    // Do dup
    fd1 = open(argv[1], O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
    fd2 = dup(fd1);
    if (fd1 == -1 || fd2 == -1) {
        printf("failed to open file");
    }

    // Flags check
    int fd1_flags = fcntl(fd1, F_GETFL);
    int fd2_flags = fcntl(fd2, F_GETFL);
    printf("[FLAG_CKECK]: fd1_flags: %d, fd2_flags: %d\n", fd1_flags, fd2_flags);

    // Do seek check
    if (lseek(fd1, 1, SEEK_SET) == -1) {
        printf("failed to seek file");
    }
    printf("[SEEK_CKECK]: fd1_offset: %lld, fd2_offset: %lld\n", GetOffset(fd1), GetOffset(fd2));

    // Do write check
    if (write(fd1, "test", 4) == -1) {
        printf("failed to write \"test\" to file");
    }
    printf("[WRITE_CKECK]: fd1_offset: %lld, fd2_offset: %lld\n", GetOffset(fd1), GetOffset(fd2));

    close(fd1);
    close(fd2);

    return 0;
}
