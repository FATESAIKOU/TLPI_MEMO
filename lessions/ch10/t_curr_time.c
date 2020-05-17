#include <stdio.h>
#include <time.h>

#define BUF_SIZE 1000

char *currTime(const char *format);


char *
currTime(const char *format)
{
    static char buf[BUF_SIZE]; /* nonreentrant */
    time_t t;
    size_t s;
    struct tm *tm;

    t = time(NULL);
    tm = localtime(&t);
    if (tm == NULL)
        return NULL;
    
    s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

    return (s == 0) ? NULL : buf;
}

int
main(int argc, char *argv[])
{
    printf("%s\n", currTime(NULL));
    return 0;
}
