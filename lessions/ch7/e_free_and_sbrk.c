#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ALLOCS 1000000

int
main(int argc, char *argv[])
{
    char **ptr = (char **)malloc(sizeof(char*) * MAX_ALLOCS);
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

    printf("\n");

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s num-allocs block-size [step [min [max]]]\n", argv[0]);
        return 0;
    }

    numAllocs = atoi(argv[1]);

    if (numAllocs > MAX_ALLOCS) {
        printf("num-allocs > %d\n", MAX_ALLOCS);
        return -1;
    }
    
    blockSize = atoi(argv[2]);
    
    freeStep = (argc > 3) ? atoi(argv[3]) : 1;
    freeMin  = (argc > 4) ? atoi(argv[4]) : 1;
    freeMax  = (argc > 5) ? atoi(argv[5]) : 1;
    
    if (freeMax > numAllocs) {
        printf("free-max > num-allocs\n");
        return -1;
    }

    printf("Initial program break           %10p\n", sbrk(0));
    
    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
    for (j = 0; j < numAllocs; j += 1) {
        ptr[j] = (char *)malloc(blockSize);
        printf("Now program break is            %10p\n", sbrk(0));
        if (ptr[j] == NULL) {
            printf("malloc error!\n");
            exit(-1);
                
        }
        printf("Program break is now:           %10p\n", sbrk(0));
    }



    printf("Freeing blocks from %d to %d in step of %d\n",
            freeMin, freeMax, freeStep);
    for (j = freeMin - 1; j < freeMax; j += freeStep)
        free(ptr[j]);

    printf("After free(), program break is: %10p\n", sbrk(0));

    exit(EXIT_SUCCESS);
}
