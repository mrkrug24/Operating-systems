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
main(int argc, const char *argv[])
{
    int status;
    pid_t pid = fork();
    
    if (!pid) {
        int fd1 = open(argv[2], O_RDONLY);
        int fd2 = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660);
        int fd3 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660);

        if (fd1 == -1 || fd2 == -1 || fd3 == -1 || 
            dup2(fd1, 0) == -1 || dup2(fd2, 1) == -1 || dup2(fd3, 2) == -1 ||
            close(fd1) == -1 || close(fd2) == -1 || close(fd3) == -1 ||
            execlp(argv[1], argv[1], NULL) == -1) {
            _exit(42);
        }

        _exit(0);
    }

    wait(&status);
    printf("%d\n", status);

    return 0;
}