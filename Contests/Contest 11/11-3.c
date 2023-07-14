#include <unistd.h>
#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <sys/wait.h>

// (pr1 < file1 && pr2) | pr3 >> file2
//   1      4      2       3      5

int
main(int argc, char const *argv[])
{
    int fd[2];
    pipe(fd);
    int status, file;
    
    if (!fork()) {
        close(fd[0]);

        if (!fork()) {
            file = open(argv[4], O_RDONLY);
            dup2(fd[1], 1);
            dup2(file, 0);
            close(fd[1]);
            close(file);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }

        wait(&status);
        if (!(WIFEXITED(status) && !WEXITSTATUS(status))) {
            _exit(1);
        }

        if (!fork()) {
            dup2(fd[1], 1);
            close(fd[1]);
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }

        wait(&status);
        if (!(WIFEXITED(status) && !WEXITSTATUS(status))) {
            _exit(1);
        }

        _exit(0);
    }

    if (!fork()) {
        file = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0700);
        dup2(fd[0], 0);
        dup2(file, 1);
        close(fd[0]);
        close(fd[1]);
        close(file);
        execlp(argv[3], argv[3], NULL);
        _exit(1);
    }

    close(fd[0]);
    close(fd[1]);
    
    while(wait(NULL) != -1) {};

    return 0;
}