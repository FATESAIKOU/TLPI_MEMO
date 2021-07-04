#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#define TAR_NEWLINE 10
#define MMAP_BLOCK_SIZE 8192 // No lower than 4096!
#define READ_BUFF_SIZE 8

// Return value:
//  Positive: found the amount of newline in the block, but not enough.
//  Negative: found the last-nth newline a the (positive)amount of offset.
int find_last_nth_newline(unsigned char *f, size_t size, int target_num)
{
    int cur_num = 0;
    for (int i = size - 1; i >= 0; i --) {
        if (f[i] == '\n') {
            cur_num ++;
        }

        if (cur_num == target_num + 1) {
            return -(i+1);
        }
    }

    return cur_num;
}

int main(int argc, char *argv[])
{
    // Open file to tail
    int src;
    src = open(argv[1], O_RDONLY);

    // Get info of file
    struct stat s;
    int status = fstat(src, &s);
    int file_size = s.st_size;
    
    printf("File size: %d\n", file_size);

    // Do mmap to the src file
    unsigned char *f;
    int cur_newline = 0;
    int tmp_newline;
    int map_offset;
    int map_size;
    int tail_offset = file_size % MMAP_BLOCK_SIZE;

    for (int i=0; 1; i ++) {
        map_offset = file_size - (tail_offset + MMAP_BLOCK_SIZE * i);
        map_size = (file_size - map_offset > MMAP_BLOCK_SIZE ?
                MMAP_BLOCK_SIZE : file_size - map_offset);

        f = (char *) mmap(NULL, map_size, PROT_READ, MAP_PRIVATE, src, map_offset);
        tmp_newline = find_last_nth_newline(f, MMAP_BLOCK_SIZE, TAR_NEWLINE - cur_newline);
        munmap(f, map_size);

        if (tmp_newline > 0) {
            cur_newline += tmp_newline;
        } else if (tmp_newline <= 0) {
            break;
        }

        if (map_offset == 0 ) 
            break;
    }

    int output_offset = map_offset + (-tmp_newline);

    // Outpur
    printf("%d\n", output_offset);
    int cur_offset = output_offset;
    ssize_t readed;
    char buf[READ_BUFF_SIZE + 1];

    lseek(src, output_offset, SEEK_SET);
    for (; cur_offset <= file_size; cur_offset += READ_BUFF_SIZE) {
        readed = read(src, buf, READ_BUFF_SIZE);
        buf[readed] = '\0';
        write(STDOUT_FILENO, buf, strlen(buf));
    }
    
    // Close fd
    close(src);
    return 0;
}
