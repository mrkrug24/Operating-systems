#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void
my_exit(pid_t *list, int n, int fd_r, int fd_w, int fd_rn)
{
    for (int i = 1; i < n; i++) {
        kill(list[i], SIGKILL);
    }
    
    while(wait(NULL) != -1);
    (fd_rn != 0) ? (close(fd_rn)) : 0;
    close(fd_r);
    close(fd_w);
    free(list);
    return;
}

int
main(int argc, char const *argv[])
{
    int fd[2], fd_r = 0;
    pid_t *list = calloc(argc + 1, sizeof(*list)), cur_pid;

    for (int i = 1; i < argc; i++) {
        if (pipe(fd) != 0) {
            my_exit(list, i, fd[0], fd[1], fd_r);
            return 1;
        }

        cur_pid = fork();
        list[i] = cur_pid;

        if (cur_pid < 0) {
            my_exit(list, i, fd[0], fd[1], fd_r);
            return 1;

        } else if (cur_pid == 0) {
            if (i != argc - 1) {
                dup2(fd[1], 1);
            }

            if ((i == argc - 1) || (i != 1 || argc == 2)) {
                dup2(fd_r, 0);
            }
            
            close(fd[0]);
            close(fd[1]);
            (fd_r != 0) ? (close(fd_r)) : 0;
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }

        close(fd[1]);
        (fd_r != 0) ? (close(fd_r)) : 0;
        fd_r = fd[0];
    }

    (fd_r != 0) ? (close(fd_r)) : 0;
    while(wait(NULL) != -1);
    free(list);

    return 0;
}