#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fptr;
    char c;

    fptr = freopen("r_open.txt", "w", stdout);
    if (!fptr) {
        printf("Failed to create file: r_open.txt");
        exit(1);
    }

    printf("Nature, time, and patience are three great physicians.\n");
    printf("Easier said than done.\n");
    printf("To teach a fish how to swim.\n");
    fclose(fptr);

    return 0;
}
