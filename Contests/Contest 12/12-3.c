#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    errno = 0;
    pid_t pid;
    char *eptr = NULL;
    long n = strtol(argv[1], &eptr, 10);

    if (errno || *eptr || eptr == argv[1] || (long) n != n) {
        exit(1);
    }

    FILE* file;
    int status, res = 0, last_parallel = 1;
    char name[PATH_MAX + 1] = {};
    
    for (int i = 2; i < n + 2 && i < argc; i++) {
        if (!fork()) {
            if ((file = fopen(argv[i], "r")) == NULL) {
                _exit(1);
            }

            fscanf(file, "%s", name);
            fclose(file);
            execlp(name, name, NULL);
            _exit(1);
        }

        last_parallel = i;
    }

    while (wait(&status) != -1) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            res += 1;
        }
    }

    for (int i = last_parallel + 1; i < argc; i++) {
        if ((pid = fork()) == 0) {
            if ((file = fopen(argv[i], "r")) == NULL) {
                _exit(1);
            }

            fscanf(file, "%s", name);
            fclose(file);
            execlp(name, name, NULL);
            _exit(1);
        }

        if (pid > 0) {
            wait(&status);
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                res += 1;
            }
        }
    }

    printf("%d\n", res);
    fflush(stdout);

    return 0;
}