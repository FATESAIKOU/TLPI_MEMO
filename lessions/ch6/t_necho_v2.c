#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    char **arg_ptr;

    for (arg_ptr = argv; *arg_ptr != NULL; arg_ptr ++) {
        puts(*arg_ptr);
    }

    return 0;
}
