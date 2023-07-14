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
    int x;
    int fd[2];
    pipe(fd); 

    if (fork() == 0) {
        close(fd[1]);

        if (fork() == 0) {
            long long sum = 0;
            
            while (read(fd[0], &x, sizeof(x)) == sizeof(x)) {
                sum += x;
            }

            printf("%lld\n", sum);
            fflush(stdout);
            _exit(0);
        }

        close(fd[0]);
        wait(NULL);
        _exit(0);
    }

    close(fd[0]);
    while (scanf("%d", &x) == 1) {
        write(fd[1], &x, sizeof(x));
    }

    close(fd[1]);
    wait(NULL);

    return 0;
}