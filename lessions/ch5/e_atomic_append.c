#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Test O_APPEND
 * argv[1]: filepath
 * argv[2]: write size
 * argv[3]: write symbol
 * argv[4]: x for removing O_APPEND, <None> for O_APPEND
 */

int main(int argc, char *argv[])
{
    int fd;
    int flags;
    int write_size;

    char symbol;
    
    // Set flags
    flags = O_WRONLY | O_CREAT | O_TRUNC | O_APPEND;
    if (argc == 5 && strcmp(argv[4], "x") == 0) {
        flags &= ~O_APPEND;
    }

    // Open file
    fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("failed to open file");
    }

    // Seek
    if (lseek(fd, 0, SEEK_END) == -1) {
        printf("failed to seek file");
    }

    // Set write parameter
    write_size = atoll(argv[2]);
    symbol = argv[3][0];

    // Write data
    int i;
    for (i = 0; i < write_size; ++ i) {
        write(fd, &symbol, 1);
    }

    close(fd);

    return 0;
}
