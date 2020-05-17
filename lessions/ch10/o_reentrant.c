#include <signal.h>
#include <stdio.h>
#include <unistd.h>

struct two_int { int a, b; } data;
void signal_handler(int signum)
{
    if (data.a != data.b)
        printf ("%d, %d\n", data.a, data.b);

    alarm(1);
}
int main (void)
{
    static struct two_int zeros = { 0, 0 }, ones = { 1, 1 };
    signal (SIGALRM, signal_handler);
    data = zeros;
    alarm (1);
    while (1)
    {
        //data.a = zeros.a;
        //data.b = zeros.b;
        //data.a = ones.a;
        //data.b = ones.b;
        data = zeros;
        data = ones;
    }
}
