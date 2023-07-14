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
    NUM = 3
};

int
main(int argc, const char *argv[])
{
    pid_t pid;

    for (int i = 1; i <= NUM; i++) {
        if (i == NUM) {
            printf("%d ", NUM);
            fflush(stdout);
        }

        if (i != NUM && ((pid = fork()) > 0)) {
            wait(NULL);
            
            if (i == 1) {
                printf("%d\n", i);
            } else {
                printf("%d ", i);
            }

            fflush(stdout);
            return 0;
        }

        if (pid < 0) {
            return -1;
        }
    }

    return 0;
}