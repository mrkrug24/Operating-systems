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
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char** argv){

    int n = strtol(argv[1], 0, 10);
    int live = strtol(argv[2], 0, 10);

    int *lives = (int*) mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

    key_t key;
    int fd = open("./a.txt", O_CREAT | O_TRUNC, 0666);
    if ((key = ftok("./a.txt", 'A')) == -1) {
        return 1;
    }
    close(fd);

    int semid;

    if ((semid = semget(key, n, IPC_CREAT | 0644)) == -1) {
        perror("semget: semget failed");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        semctl(semid, i, SETVAL, 0);
    }

    for (int i = 0; i < n; i++) {
        lives[i] = live;
    }

    setbuf(stdin, NULL);
    pid_t pid;
    int num, proc_num;

    int *pids = (int*) mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);

    for (int i = 0; i < n; i++) {
        if ((pid = fork()) == 0) {
            struct sembuf down;
            down.sem_flg = 0;
            down.sem_op = -1;
            down.sem_num = i;

            while (1) {
                semop(semid, &down, 1);

                printf("%d %d\n", i, lives[i]);
                fflush(stdout);

                if (lives[i] == 0) {
                    _exit(0);
                }
            }
            _exit(0);
        }
        pids[i] = pid;
    }

    int ops = 0;
    while (scanf("%d", &num) != EOF) {
        int proc_num = num % n;
        ops++;

        printf("%d\n", proc_num);
        fflush(stdout);

        struct sembuf up;
        up.sem_flg = 0;
        up.sem_op = 1;
        up.sem_num = proc_num;
        semop(semid, &up, 1);

        lives[proc_num]--;
    }

    while (wait(NULL) != -1) {};

    return 0;
}