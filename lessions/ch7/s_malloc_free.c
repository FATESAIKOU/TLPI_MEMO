#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//memory control block defination
struct mem_control_block{
        int is_available;
        int size;
};
void *my_malloc(long numbytes);//alloc numbytes memory,return the pointer to the first byte
void my_free(void *firstbyte);//release the memory to the process idle space
int has_initialized=0;
void *managed_memory_start;
void *last_valid_address;
void my_malloc_init(){
        //grab the last valid address from the OS
        last_valid_address=sbrk(0);
        //set the begining to the last_valid_address
        managed_memory_start=last_valid_address;
        //flag to has_initialized
        has_initialized=1;
}
void my_free(void *firstbyte){
        struct mem_control_block *mcb;
        mcb=firstbyte-sizeof(struct mem_control_block);
        mcb->is_available=1;
        return;
}
void *my_malloc(long numbytes){
        //holds where we are looking for in memory
        void *current_location;
        //same as current_location
        struct mem_control_block *current_location_mcb;
        //return memory location .look until we find sth suitable
        void *memory_location;
        //init if not ed
        if(!has_initialized){
                my_malloc_init();
        }

        //total memory to find,including mem_control_block is just a flag
        numbytes+=sizeof(struct mem_control_block);
        //set memory_location to 0 until we find sth suitable
        memory_location=0;
        //begin sear at the start of managed memory
        current_location=managed_memory_start;
        /*keep going until we have searched all allocated space*/
        while(current_location!=last_valid_address){
                current_location_mcb=(struct mem_control_block*)current_location;
                if(current_location_mcb->is_available){
                        if(current_location_mcb->size >= numbytes){
                                //got it
                                current_location_mcb->is_available=0;
                                memory_location=current_location;
                                break;
                        }
                }
                //move to the nes memory
                current_location+=current_location_mcb->size;
        }
        //ask OS
        if(!memory_location){
                //move the program break numbyes further
                sbrk(numbytes);
                memory_location=last_valid_address;
                last_valid_address+=numbytes;
                //init memory_control_block
                current_location_mcb=memory_location;
                current_location_mcb->is_available=0;
                current_location_mcb->size=numbytes;
        }
        //move the pointer past the mem_control_block
        memory_location+=sizeof(struct mem_control_block);
        return memory_location;
}
int main(){
        char *p=(char*)my_malloc(sizeof(char)*1000);
        my_free(p);
        return 1;
}
