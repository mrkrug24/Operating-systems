#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int
main(int argc, const char *argv[])
{   
    int x;
    pid_t pid;
    int status;
    
    if ((pid = fork()) == 0) {
        while (scanf("%d", &x) == 1) {
            if ((pid = fork()) > 0) {
                wait(&status);

                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    printf("%d\n", x);
                    fflush(stdout);
                    return 0;
                }

                return 1;
            } 

            if (pid < 0) {
                printf("%d\n", -1);
                return 1;
            }
        }

    } else {
        wait(NULL);
    }

    return 0;
}