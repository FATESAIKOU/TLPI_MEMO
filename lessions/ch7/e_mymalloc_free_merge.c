#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ALLOCS 1000000

struct mem_control_block {
    int is_available;
    int size;
};

void my_malloc_init(void);
void *my_malloc(size_t size);
void my_free(void *ptr);
void *get_next_continous_space(void *current_location, void *last_valid_address);


int
main(int argc, char *argv[])
{
    char **ptr = (char **)my_malloc(sizeof(char*) * MAX_ALLOCS);
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

    printf("Initial program breakA          %10p\n", sbrk(0));
    
    for (j = 0; j < numAllocs*2; j += 1) {
        ptr[j] = (char *)my_malloc(blockSize / 2);
        if (ptr[j] == NULL) {
            printf("malloc error!\n");
            exit(-1);
                
        }
        printf("Program break is now:           %10p\n", sbrk(0));
    }
    for (j = freeMin - 1; j < freeMax*2; j += freeStep)
        my_free(ptr[j]);
    
    
    printf("Initial program breakB          %10p\n", sbrk(0));
    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
    for (j = 0; j < numAllocs; j += 1) {
        ptr[j] = (char *)my_malloc(blockSize);
        if (ptr[j] == NULL) {
            printf("malloc error!\n");
            exit(-1);
                
        }
        printf("Program break is now:           %10p\n", sbrk(0));
    }

    printf("Freeing blocks from %d to %d in step of %d\n",
            freeMin, freeMax, freeStep);
    for (j = freeMin - 1; j < freeMax; j += freeStep)
        my_free(ptr[j]);

    printf("After free(), program break is: %10p\n", sbrk(0));

    exit(EXIT_SUCCESS);
}


int has_initialized = 0;
void *managed_memory_start;
void *last_valid_address;

void
my_malloc_init()
{
    last_valid_address = sbrk(0);
    managed_memory_start = last_valid_address;
    has_initialized = 1;
}

void *my_malloc(size_t size)
{
    void *current_location;
    struct mem_control_block *current_location_mcb, *remain_space_mcb;
    void *memory_location;

    if (!has_initialized)
        my_malloc_init();

    size += sizeof(struct mem_control_block);
    memory_location = 0;
    current_location = managed_memory_start;

    while (current_location != last_valid_address) {
        current_location_mcb = (struct mem_control_block*)get_next_continous_space(current_location, last_valid_address);
        if (current_location_mcb->is_available) {
            if (current_location_mcb->size >= size) {
                current_location_mcb->is_available = 0;
                
                if (current_location_mcb->size >= size + sizeof(struct mem_control_block)) {

                    remain_space_mcb = (struct mem_control_block*)(current_location + size);
                    remain_space_mcb->is_available = 1;
                    remain_space_mcb->size = current_location_mcb->size - size;
                    current_location_mcb->size = size;
                }

                memory_location = current_location;
                break;
            }
        }

        current_location += current_location_mcb->size;
    }

    if (!memory_location) {
        printf("fuck! it called the sbrk!\n");
        sbrk(size);
        memory_location = last_valid_address;
        last_valid_address += size;
        
        current_location_mcb = memory_location;
        current_location_mcb->is_available = 0;
        current_location_mcb->size = size;
    }

    memory_location += sizeof(struct mem_control_block);

    return memory_location;
}

void my_free(void *ptr)
{
    if (ptr == NULL)
        return;

    struct mem_control_block *free;

    free = ptr - sizeof(struct mem_control_block);
    free->is_available = 1;
    
    return;
}

void *get_next_continous_space(void *current_location, void *last_valid_address)
{
    
    struct mem_control_block *base_mcb_ptr, *pivot_mcb_ptr;

    base_mcb_ptr = (struct mem_control_block*)current_location;
    pivot_mcb_ptr = base_mcb_ptr;
    do {
        if (!pivot_mcb_ptr->is_available) {
            return base_mcb_ptr;
        } else if (pivot_mcb_ptr != base_mcb_ptr) {
            base_mcb_ptr->size += pivot_mcb_ptr->size;
            printf("[[[Call merged!]]]\n");
        }

        pivot_mcb_ptr = ((void *)pivot_mcb_ptr) + pivot_mcb_ptr->size;

    } while ( ((void*)pivot_mcb_ptr) < last_valid_address);

    return base_mcb_ptr;
}
