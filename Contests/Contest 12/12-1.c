#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

volatile int cnt = 0;

void
action(int signum)
{
    signal(SIGHUP, action);

    if (cnt == 5) {
        _exit(0);
    }

    printf("%d\n", cnt++);
    fflush(stdout);

    return;
}

int
main(int argc, char const *argv[])
{
    signal(SIGHUP, action);
    printf("%d\n", getpid());
    fflush(stdout);
    while(1) pause();
    return 0;
}