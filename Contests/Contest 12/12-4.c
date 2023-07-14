#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

volatile int check = 0;

void action(int signum)
{
    signal(SIGUSR1, action);
    check = 1;
}

int
main(int argc, char *argv[])
{
    sigset_t mask, mask_tmp;
    sigemptyset(&mask);
    sigaddset(&mask_tmp, SIGHUP);
    sigprocmask(SIG_BLOCK, &mask_tmp, &mask);
    signal(SIGUSR1, action);
    sigemptyset(&mask_tmp);
    
    errno = 0;
    char *eptr = NULL;
    long n = strtol(argv[1], &eptr, 10);

    if (errno || *eptr || eptr == argv[1] || (long) n != n) {
        exit(1);
    }

    pid_t pid_1, pid_2, pid_cur;
    int cnt, pid, fd[2];
    pipe(fd);
    
    FILE *fd_r = fdopen(fd[0], "r");
    FILE *fd_w = fdopen(fd[1], "w");
    setbuf(fd_r, 0);
    setbuf(fd_w, 0);

    for (int i = 1; i <= 2; i++) {
        if (i == 1) {
            pid_1 = fork();
            pid_cur = pid_1;
        } else {
            pid_2 = fork();
            pid_cur = pid_2;
        }

        if (pid_cur == 0) {
            if (i == 2) {
                while (check == 0) {
                    sigsuspend(&mask);
                }
                check = 0;
            }

            fscanf(fd_r, "%d", &pid);
            kill(pid, SIGUSR1);

            while (1) {
                while (check == 0) {
                    sigsuspend(&mask);
                }

                check = 0;
                fscanf(fd_r, "%d", &cnt);

                if (cnt >= n) {
                    kill(pid, SIGUSR1);
                    fprintf(fd_w, "%d\n", cnt);
                    fflush(fd_w);
                    _exit(0);
                } else {
                    fprintf(fd_w, "%d\n", cnt + 1);
                    fflush(fd_w);
                    printf("%d %d\n", i, cnt);
                    fflush(stdout);
                }

                kill(pid, SIGUSR1);
            }
        }
    }

    fprintf(fd_w, "%d\n%d\n%d\n", pid_2, pid_1, 1);
    fflush(fd_w);
    fclose(fd_r);
    fclose(fd_w);
    
    while(wait(NULL) != -1) {};
    close(fd[0]);
    close(fd[1]);
    printf("Done\n");

    return 0;
}