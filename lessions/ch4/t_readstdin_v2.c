#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_READ 20

int main()
{
    char buffer[MAX_READ] = "foo";
    char buffer2[MAX_READ] = "bar";
    printf("Addr: %p, %p\n", buffer, buffer2);
    printf("Data: %s, %s\n", buffer, buffer2);
   
    int num_read = read(STDIN_FILENO, buffer, 40);
    if (num_read == -1) {
        printf("Error: read\n");
        exit(1);
    }

    buffer[num_read] = '\0';

    printf("The input data was: %s\n", buffer);
    printf("Buffer2: %s\n", buffer2);

    return 0;
}
