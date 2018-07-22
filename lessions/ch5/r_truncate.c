#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    // Test truncate
    if (truncate(argv[1], 16) != 0) {
        printf("Truncate %s: error\n", argv[1]);
        exit(-1);
    }

    printf("truncate success\n");

    // Test ftruncate
    int fd = open(argv[2], O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("failed to open %s\n", argv[2]);
        exit(-1);
    }

    if (ftruncate(fd, 32) != 0) {
        printf("Ftruncate %s: error\n", argv[2]);
        exit(-1);
    }

    printf("ftruncate success\n");
    
    return 0;
}
