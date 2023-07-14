#include <limits.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char** argv){

    int fd = open(argv[1], O_RDONLY);
    int fd_write = open(argv[1], O_WRONLY);

    dup2(fd, 0);
    close(fd);

    pid_t pid;

    if ((pid = fork()) == 0) {
        dup2(fd_write, 1);
        close(fd_write);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }
    wait(NULL);
    close(fd_write);

    return 0;
}