#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

int
main(int argc, const char *argv[])
{
    errno = 0;
    char *eptr = NULL;
    long n = strtol(argv[1], &eptr, 10);

    if (errno || *eptr || eptr == argv[1] || (long) n != n) {
        exit(1);
    }

    int fd_1[2], fd_2[2];
    pipe(fd_1);
    pipe(fd_2);

    FILE *fd_1_r = fdopen(fd_1[0], "r");
    FILE *fd_1_w = fdopen(fd_1[1], "w");
    FILE *fd_2_r = fdopen(fd_2[0], "r");
    FILE *fd_2_w = fdopen(fd_2[1], "w");

    int cnt = 1;
    fprintf(fd_1_w, "%d\n", cnt);
    fflush(fd_1_w);

    if (fork() == 0) {
        close(fd_1[1]);
        close(fd_2[0]);
        fclose(fd_1_w);
        fclose(fd_2_r);

        while (fscanf(fd_1_r,"%d", &cnt) == 1) {
            if (cnt < n) {
                printf("1 %d\n", cnt);
                fflush(stdout);
            } else {
                _exit(0);
            }

            if (++cnt >= n) {
                _exit(0);
            }

            fprintf(fd_2_w, "%d\n", cnt);
            fflush(fd_2_w);
        }

        _exit(0);
    }

    if (fork() == 0) {
        close(fd_1[0]);
        close(fd_2[1]);
        fclose(fd_1_r);
        fclose(fd_2_w);
        
        while (fscanf(fd_2_r,"%d", &cnt) == 1) {
            printf("2 %d\n", cnt);
            fflush(stdout);

            if (++cnt >= n) {
                _exit(0);
            }

            fprintf(fd_1_w, "%d\n", cnt);
            fflush(fd_1_w);
        }

        _exit(0);
    }

    close(fd_1[0]);
    close(fd_1[1]);
    close(fd_2[1]);
    close(fd_2[0]);
    fclose(fd_1_w);
    fclose(fd_1_r);
    fclose(fd_2_w);
    fclose(fd_2_r);

    while(wait(NULL) != -1) {};
    printf("Done\n");

    return 0;
}