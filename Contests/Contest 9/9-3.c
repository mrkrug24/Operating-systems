#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

enum
{
    NUM = 3,
    SIZE = 8
};

int
main(int argc, const char *argv[])
{
    pid_t pid;
    
    for (int i = 1; i <= NUM; i++) {
        if ((pid = fork()) == 0) {
            char buf[SIZE] = {};
            read(0, buf, sizeof(buf));

            long long x;
            char *eptr = NULL;
            x = strtoll(buf, &eptr, 10);

            printf("%d %lld\n", i, x * x);
            fflush(stdout);

            return 0;
        }
    }

    while (wait(NULL) != -1);

    return 0;
}