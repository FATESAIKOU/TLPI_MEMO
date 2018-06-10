#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../lib/tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int inputFD, outputFD, output_openFlags;
    mode_t output_filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        // usageErr("%s src copy_dest\n", argv[0]);
        printf("%s src copy_dest\n", argv[0]);
    
    /* Open File */
    inputFD = open(argv[1], O_RDONLY);
    if (inputFD == -1)
    {
        // errExit("opening file %s", argv[1]);
        printf("opening file %s", argv[1]);
        exit(-1);
    }

    output_openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    output_filePerms = S_IRUSR | S_IWUSR |
                       S_IRGRP | S_IWGRP |
                       S_IROTH | S_IWOTH;
                       /* rw-rw-rw- */
    outputFD = open(argv[2], output_openFlags, output_filePerms);
    if (outputFD == -1)
    {
        // errExit("opening file %s", argv[2]);
        printf("opening file %s", argv[2]);
        exit(-1);
    }

    /* Transfer Data */
    while ((numRead = read(inputFD, buf, BUF_SIZE)) > 0)
        if (write(outputFD, buf, numRead) != numRead)
        {
            // fatal("couldn't write whole buffer");
            printf("couldn't write whole buffer");
            exit(-1);
        }

    if (numRead == -1)
    {
        // errExit("read");
        printf("read");
        exit(-1);
    }

    if (close(inputFD) == -1)
    {
        // errExit("close input");
        printf("close input");
        exit(-1);
    }
    if (close(outputFD) == -1)
    {
        // errExit("close output");
        printf("close output");
        exit(-1);
    }
    
    exit(0);
}
