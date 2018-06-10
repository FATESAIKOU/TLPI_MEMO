#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int new_fd = open("r_open.txt", O_RDWR|O_CREAT);

    if (new_fd == -1) {
        printf("Failed to create file: r_open.txt\n");
        exit(1);
    }

    // Backup stdout fileno
    int stdout_fd = dup(STDOUT_FILENO);
    
    // Change STDOUT_FILENO's open_file_id to new_fd.
    dup2(new_fd, STDOUT_FILENO);
    close(new_fd);

    printf("Nature, time, and patience are three great physicians.\n");
    printf("Easier said than done.\n");
    printf("To teach a fish how to swim.\n");

    fflush(stdout);

    // Restore STDOUT_FILENO.
    dup2(stdout_fd, STDOUT_FILENO);
    
    printf("Hello, World!\n");

    return 0;
}
