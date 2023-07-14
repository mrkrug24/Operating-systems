#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>

int
main(int argc, char *argv[]) 
{
    errno = 0;
    int x, next;
    char *eptr = NULL;
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    struct sembuf up = {0, 1, 0};
    int n = strtol(argv[1], &eptr, 10);

    if (errno || *eptr || eptr == argv[1] || (int) n != n) {
        _exit(1);
    }

    int sem_id = semget(IPC_PRIVATE, n, IPC_CREAT | 0666);

    for (int i = 0; i < n; i++) {
        if (fork() == 0) {
            struct sembuf down = {i, -1, 0};
            while (semop(sem_id, &down, 1) == 0) {
                (scanf("%d", &x) == 1) ? (printf("%d %d\n", i, x)) : (semctl(sem_id, 0, IPC_RMID, 0), _exit(0));
                next = (x % n >= 0) ? (x % n) : (x % n + n);
                struct sembuf up_next = {next, 1, 0};
                semop(sem_id, &up_next, 1);
            }

            _exit(0);
        }
    }

    semop(sem_id, &up, 1);
    while (wait(NULL) != -1);

    return 0;
}