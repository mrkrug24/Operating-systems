#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>


volatile sig_atomic_t flag = 0;

enum { SIG_USR1 = 1, SIG_USR2 = 2, SIG_ALARAM = 3, SIG_IO = 4, DFL = 1 };

void handler(int s) {
    if (s == SIGUSR1) {
        flag = SIG_USR1;
    } else if (s == SIGUSR2) {
        flag = SIG_USR2;
    } else {
        flag = DFL;
        if (s == SIGIO) {
            _exit(0);
        }
    }
}

int main(int argc, char** argv) {

    if (argc < 2) {
        return 1;
    }

    sigset_t s1, s2, s3;
    sigemptyset(&s1); sigaddset(&s1, SIGUSR1); sigaddset(&s1, SIGUSR2); sigaddset(&s1, SIGIO);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGIO, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    sigemptyset(&s1);
    sigaddset(&s1, SIGALRM);
    sigprocmask(SIG_BLOCK, &s1, &s3);
    sigaction(SIGALRM, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    int pipefd[2];
    pipe(pipefd);

    pid_t pid_1, pid_2;

    if ((pid_1 = fork()) == 0) {
        close(pipefd[1]);
        int pid;
        read(pipefd[0], &pid, sizeof(pid));
        close(pipefd[0]);

        char buf = 0;
        int len = 0;

        while (1) {
            while (!flag) {
                sigsuspend(&s2);
            }

            if (flag == SIG_USR1) {
                buf |= (0 << len);
                len++;
            } else if (flag == SIG_USR2) {
                buf |= (1 << len);
                len++;
            }
            if (len == CHAR_BIT) {
                putchar(buf);
                fflush(stdout);
                buf = 0;
                len = 0;
            }

            flag = 0;
            kill(pid, SIGALRM);
        }
    } else {
        if ((pid_2 = fork()) == 0) {
            close(pipefd[0]);
            close(pipefd[1]);

            int fd;
            if ((fd = open(argv[1], O_RDONLY)) < 0) {
                kill(pid_1, SIGIO);
                _exit(0);
            }
            char x;

            while (read(fd, &x, sizeof(x))) {
                for (int i = 0; i < CHAR_BIT; i++) {
                    if (((x >> i) & 1)) {
                        kill(pid_1, SIGUSR2);
                    } else {
                        kill(pid_1, SIGUSR1);
                    }

                    while (!flag) {
                        sigsuspend(&s3);
                    }
                    flag = 0;
                }
            }
            close(fd);
            kill(pid_1, SIGIO);
            _exit(0);
        }
    }
    close(pipefd[0]);
    write(pipefd[1], &pid_2, sizeof(pid_2));
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);

    sigaction(SIGALRM, &(struct sigaction) { .sa_handler = SIG_DFL, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = SIG_DFL, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = SIG_DFL, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGIO, &(struct sigaction) { .sa_handler = SIG_DFL, .sa_flags = SA_RESTART }, NULL);

    return 0;
}