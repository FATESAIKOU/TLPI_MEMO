#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>

struct passwd*
mygetpwnam(const char *name)
{
    struct passwd *pwd = NULL;

    while ( (pwd = getpwent()) != NULL ) {
        if ( strcmp(pwd->pw_name, name) == 0 )
            break;
    }
    
    endpwent();

    return pwd;
}

int
main(int argc, char *argv[])
{
    struct passwd *pwd = mygetpwnam("fatesaikou");
    printf("The uid of [fatesaikou] is  %5ld\n", (long) pwd->pw_uid);

    return 0;
}
