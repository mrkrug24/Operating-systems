#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

volatile int check = 0;

void action(int signum) {
    signal(SIGUSR1, action);
    signal(SIGUSR2, action);
    signal(SIGALRM, action);
    signal(SIGIO, action);

    if (signum == SIGUSR1) {
        check = 0;
    } else if (signum == SIGUSR2) {
        check = 1;
    } else if (signum == SIGALRM) {
        check = 2;
    } else if (signum == SIGIO) {
        check = 3;
    }
}

int main(int argc, char** argv)
{
    int fd[2], fd_pip[2];
    pipe(fd);
    pipe(fd_pip);

    FILE *fd_r = fdopen(fd[0], "r");
    FILE *fd_w = fdopen(fd[1], "w");
    setbuf(fd_r, NULL);
    setbuf(fd_w, NULL);

    sigset_t mask_1, mask_2;
    sigemptyset(&mask_1);
    sigemptyset(&mask_2);
    sigaddset(&mask_1, SIGUSR1);
    sigaddset(&mask_1, SIGUSR2);
    sigaddset(&mask_1, SIGALRM);
    sigaddset(&mask_1, SIGIO);
    sigprocmask(SIG_BLOCK, &mask_1, &mask_2);

    signal(SIGUSR1, action);
    signal(SIGUSR2, action);
    signal(SIGALRM, action);
    signal(SIGIO, action);

    pid_t pid_1, pid_2, pid_cur;
    int cnt, pid;

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
                    sigsuspend(&mask_2);
                }

                check = 0;
            }

            fscanf(fd_r, "%d", &pid);
            kill(pid, SIGUSR1);

            while (1) {
                while (check == 0) {
                    sigsuspend(&mask_2);
                }

                check = 0;
                fscanf(fd_r, "%d", &cnt);

                if (cnt == max) {
                    fprintf(fd_w, "%d\n", cnt);
                    fflush(fd_w);
                    kill(pid, SIGUSR1);
                    _exit(0);
                } else {
                    printf("%d %d\n", i, cnt);
                    fflush(stdout);
                    fprintf(fd_w, "%d\n", cnt + 1);
                    fflush(fd_w);
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
    printf("Done\n");

    return 0;
}