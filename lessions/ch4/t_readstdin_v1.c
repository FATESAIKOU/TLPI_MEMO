#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_READ 20

int main()
{
    char buffer[MAX_READ];
    
    if (read(STDIN_FILENO, buffer, MAX_READ) == -1) {
        printf("Error: read\n");
        exit(1);
    }

    printf("The input data was: %s\n", buffer);

    return 0;
}
