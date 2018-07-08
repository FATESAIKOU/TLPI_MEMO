#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    while (1) {
        sleep(1000);
        printf("On executing...\n");
    }
}
