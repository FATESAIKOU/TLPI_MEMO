#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int
main(int argc, char *argv[]) 
// For linux, we can also define the as:
//  int main (int argc, char *argv[], char *envp[])
// 
// But be aware of, altough this commonly be implemented
// at Linux, it is still not the standard defined in SUSv3.
{
    char **ep;
    
    for (ep = environ; *ep != NULL; ep ++) {
        puts(*ep);
    }

    return 0;
}
