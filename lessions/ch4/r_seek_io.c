#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

void Display(char *cmd, int fd, long int len);

void Write(char *cmd, int fd, char *write_string);

void Seek(char *cmd, int fd, off_t offset);

void ErrShow(const char* format, ...);

void ErrExit(const char* format, ...);


int main(int argc, char *argv[])
{
    size_t len;
    off_t offset;
    int fd, ap;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        ErrExit("%s seek_io {r<length>|R<length>|w<string>}|s<offset>}...\n",
                argv[0]);

    fd = open(argv[1], O_RDWR | O_CREAT,
                S_IRUSR | S_IWUSR |         /* rw-rw-rw- */
                S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH); 

    if (fd == -1)
        ErrExit("open");

    for (ap = 2; ap < argc; ap++) {
        switch (argv[ap][0]) {
            case 'r': /* Display bytes at current offset, as text */
            case 'R': /* Display bytes at current offset, in hex */
                len = atol(&argv[ap][1]);
                Display(argv[ap], fd, len);
                break;
            case 'w': /* Write string at current offset */
                Write(argv[ap], fd, &argv[ap][1]);
                break;
            case 's': /* Change file offset */
                offset = atol(&argv[ap][1]);
                Seek(argv[ap], fd, offset);
                break;
            default:
                ErrShow("Argumemt must start with [rRws]: %s\n", argv[ap]);
        }
    }

    if (close(fd) == -1)
        ErrExit("close");

    return 0;
}

void Display(char *cmd, int fd, long int len)
{
    int i;
    ssize_t num_read;
    char *buf = malloc(len);

    if (buf == NULL)
        ErrExit("unable to malloc buffer of size %ld.\n", len);

    num_read = read(fd, buf, len);
    if (num_read == -1)
        ErrExit("failed to read.\n");

    if (num_read == 0) {
        printf("%s: end-of-file\n", cmd);
    } else {
        printf("%s: ", cmd);
        if (cmd[0] == 'r') {
            for (i = 0; i < num_read; i++)
                printf("%c", isprint((unsigned char) buf[i]) ?
                                        buf[i] : '?');
        } else {
            for (i = 0; i < num_read; i++)
                printf("%02x", (unsigned int) buf[i]);
        }
        printf("\n");
    }

    free(buf);
}

void Write(char *cmd, int fd, char *write_string)
{
    ssize_t num_written = write(fd, write_string, strlen(write_string));
    if (num_written == -1)
        ErrExit("failed to write.\n");
    printf("%s: wrote %ld bytes\n", cmd, num_written);
}

void Seek(char *cmd, int fd, off_t offset)
{
    if (lseek(fd, offset, SEEK_SET) == -1)
        ErrExit("unable to seek.\n");
    printf("%s: seek succeeded\n", cmd);
}

void ErrShow(const char* format, ...)
{
    printf("ResumableError: ");
    
    va_list marker;
    va_start(marker, format);
    vprintf(format, marker);
    va_end(marker);
}

void ErrExit(const char* format, ...)
{
    printf("Error: ");
    
    va_list marker;
    va_start(marker, format);
    vprintf(format, marker);
    va_end(marker);

    exit(-1);
}
