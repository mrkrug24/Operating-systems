#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/wait.h>

typedef struct Message
{
    long long mtype;
    long long data[2];
} Message;

int
main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    long long key = strtoll(argv[1], NULL, 10);
    long long n = strtoll(argv[2], NULL, 10);
    long long val_1 = strtoll(argv[3], NULL, 10);
    long long val_2 = strtoll(argv[4], NULL, 10);
    long long max = strtoll(argv[5], NULL, 10);
    
    pid_t pid;
    Message msg;
    long long x_1, x_2, x_3;
    int msg_id = msgget(key, IPC_CREAT | 0666);

    for (long long i = 1; i <= n; i++) {
        if ((pid = fork()) == 0) {
            while (msgrcv(msg_id, &msg, sizeof(msg), i, 0) >= 0) {
                x_1 = msg.data[0];
                x_2 = msg.data[1];
                x_3 = x_1 + x_2;

                printf("%lld %lld\n", i - 1, x_3);
                
                if (x_3 > abs(max)) {
                    msgctl(msg_id, IPC_RMID, NULL);
                    _exit(0);
                }

                msg.mtype = x_3 % n + 1;
                msg.data[0] = x_2;
                msg.data[1] = x_3;
                msgsnd(msg_id, &msg, sizeof(msg), 0);
            }

            _exit(0);
            
        } else if (pid < 0) {
            msgctl(msg_id, IPC_RMID, NULL);
            while (wait(NULL) != -1);
            return 1;
        }
    }

    msg.mtype = 1;
    msg.data[0] = val_1;
    msg.data[1] = val_2;
    msgsnd(msg_id, &msg, sizeof(msg), 0);
    while (wait(NULL) != -1);

    return 0;
}