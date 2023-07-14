#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/wait.h>

int
check(const char *file)
{
    int status;
    pid_t pid = fork();

    if (pid < 0) {
        return 0;

    } else if (pid > 0) {
        wait(&status);

        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            return 1;
        }

    } else {
        execlp(file, file, NULL);
        _exit(1);
    }

    return 0;
}

int
main(int argc, const char *argv[])
{   
    return !((check(argv[1]) || check(argv[2])) && check(argv[3]));
}