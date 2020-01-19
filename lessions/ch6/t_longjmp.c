#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;
static void
f2(void)
{
    longjmp(env, 2);
}

static void
f1(int argc)
{
    if (argc == 1) {
        longjmp(env, 1);
    }
    f2();
}


/*
 * Better
 * not
 * to
 * do!
 */
int
f0(void)
{
    int i = setjmp(env);
    return i;
}

int
main(int argc, char *argv[])
{
    // s = setjmp(env) is invalid !!!!
    //int i = f0();
    //switch (i) {
    switch (setjmp(env)) { // First call, it returns 0
    case 0:
        printf("Calling f1() after the initial setjmp().\n");
        f1(argc);
        break;
    case 1:
        printf("We jumped from f1()\n");
        break;
    case 2:
        printf("We jumped from f2()\n");
    }

    return 0;
}
