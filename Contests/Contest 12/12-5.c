#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

volatile int last = 0, cnt = 0;

void
action(int signum)
{
    signal(SIGTERM, action);
    signal(SIGINT, action);

    if (signum == SIGTERM) {
        _exit(0);
    }

    if (signum == SIGINT && ++cnt < 4) {
        printf("%d\n", last);
        fflush(stdout);
    } else if (signum == SIGINT && cnt >= 4) {
        _exit(0);
    }
}

int
prime_number(int x)
{
    if (x == 2) {
        return 1;
    } else if (x % 2 == 0) {
        return 0;
    } else {
        for (int i = 3; i < sqrt(x) + 1; i += 2) {
            if (x % i == 0) {
                return 0;
            }
        }
    }

    return 1;
}

int
main(int argc, char *argv[])
{
    signal(SIGINT, action);
    signal(SIGTERM, action);

    int low, high;
    scanf("%d%d", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);

    for (int i = low; i < high; i++) {
        if (prime_number(i) == 1) {
            last = i;
        }
    }

    printf("-1\n");
    fflush(stdout);
    return 0;
}