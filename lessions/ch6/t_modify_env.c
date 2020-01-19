#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int
main(int argc, char *argv[])
{
    int i;
    char **ep;

    clearenv(); // Erase entire environment

    putenv("a=2");
    for (i = 1; i < argc; i ++)
        if (putenv(argv[i]) != 0) {
            printf("putenv: %s, error\n", argv[i]);
            return -1;
        }

    if (setenv("GREET", "Hello, world", 0) == -1) {
        printf("setenv error\n");
        return -1;
    }

    unsetenv("BYE");

    for (ep = environ; *ep != NULL; ep ++)
        puts(*ep);

    return 0;
}
