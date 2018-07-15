#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _b : _a; })

void CopyData(int src, off_t prev, off_t now, int dest);

void MakeHole(int dest, off_t lenght);

int main(int argc, char *argv[])
{
    int src_fd = open(argv[1], O_RDONLY,
                        S_IRUSR | S_IWUSR |         /* rw-rw-rw- */
                        S_IRGRP | S_IWGRP |
                        S_IROTH | S_IWOTH); 

    int des_fd = open(argv[2], O_WRONLY | O_CREAT,
                        S_IRUSR | S_IWUSR |         /* rw-rw-rw- */
                        S_IRGRP | S_IWGRP |
                        S_IROTH | S_IWOTH); 


    int mode = 0;
    off_t prev = 0;
    off_t end = lseek(src_fd, 0, SEEK_END);
    off_t now;

    lseek(src_fd, 0, SEEK_SET);
    while (prev < end) {
        if (mode == 0) {
            now = lseek(src_fd, prev, SEEK_DATA);
        } else {
            now = lseek(src_fd, prev, SEEK_HOLE);
        }

        if ((now - prev) > 0) {
            if (mode == 0) {
                MakeHole(des_fd, now - prev);
            } else {
                CopyData(src_fd, prev, now, des_fd);
            }
        }

        prev = now;
        mode = !mode;
    }

    close(src_fd);
    close(des_fd);

    return 0;
}

void CopyData(int src, off_t prev, off_t now, int dest)
{
    char buf[4096];
    off_t length = now - prev;
    off_t now_len = 0;
    off_t read_len = 0;

    lseek(src, prev, SEEK_SET);
    do {
        read_len = read(src, buf, min(4096, length - now_len));
        printf("Write: %ld\n", read_len);
        write(dest, buf, read_len);
        now_len += read_len;
    } while (now_len < length);
    lseek(src, now, SEEK_SET);
}

void MakeHole(int dest, off_t length)
{
    printf("Seek: %ld\n", length);
    lseek(dest, length, SEEK_CUR);
}
