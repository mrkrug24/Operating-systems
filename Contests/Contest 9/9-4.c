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

int
main(int argc, const char *argv[])
{
    int n;
    scanf("%d", &n);
    pid_t pid;

    for (int i = 1; i <= n; i++) {
        if (i == n) {
            printf("%d\n", i);
        } else {
            printf("%d ", i);
        }
        
        fflush(stdout);

        if (i != n && ((pid = fork()) > 0)) {
            wait(NULL);
            return 0;
        }

        if (pid < 0) {
            return -1;
        }
    }

    return 0;
}