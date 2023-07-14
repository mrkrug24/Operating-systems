#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int n = strtol(argv[1], NULL, 10);
    int key = strtol(argv[2], NULL, 10);
    int max = strtol(argv[3], NULL, 10);

    int shm_id = shmget(key, 2 * sizeof(max), IPC_CREAT | 0666);
    int *mem = shmat(shm_id, NULL, 0);
    int sem_id = semget(key, n + 1, IPC_CREAT | 0666);
    mem[0] = 0;
    mem[1] = 0;
    
    for (int i = 1; i <= n; i++) {
        semctl(sem_id, i, SETVAL, 0);

        if (fork() == 0) {
            struct sembuf down = {i, -1, 0};
            while (semop(sem_id, &down, 1) == 0) {
                int cur = mem[0];
                int prev = mem[1];

                printf("%d %d %d\n", i, cur, prev);
                fflush(stdout);

                if (++cur > max) {
                    semctl(sem_id, 0, IPC_RMID, 0);
                    shmctl(shm_id, IPC_RMID, NULL);
                    _exit(0);
                }

                mem[0] = cur;
                mem[1] = i;

                long long next = 1;
                for (int j = 0; j < 4; j++) {
                    next = (next * cur) % n;
                }

                struct sembuf up_next = {next + 1, 1, 0};
                semop(sem_id, &up_next, 1);
            }

            _exit(0);
        }
    }

    struct sembuf up = {1, 1, 0};
    semop(sem_id, &up, 1);
    while ((wait(NULL)) != -1);

    return 0;
}