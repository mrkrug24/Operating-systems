#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

volatile int check_action = 0;

void
action(int signum)
{
    signal(SIGINT, action);
    signal(SIGQUIT, action);
    
    if (signum == SIGINT) {
        check_action = 0;
    } 
    
    if (signum == SIGQUIT) {
        check_action = 1;
    }

    return;
}

int
main(int argc, char const *argv[])
{
    unsigned int res = 0;
    int x;

    signal(SIGINT, action);
    signal(SIGQUIT, action);
    printf("%d\n", getpid());
    fflush(stdout);

    while (scanf("%d", &x) == 1) {
        if (check_action == 0) {
            res += x;
        } 
        
        if (check_action == 1) {
            res *= x;
        }

        printf("%d\n", (int)res);
        fflush(stdout);
    }

    return 0;
}