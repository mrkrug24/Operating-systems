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
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        if (fork() == 0) {
            if (fork() == 0) {
                time_t time_cur = time(NULL);
                write(fd[1], &time_cur, sizeof(time_cur));
                write(fd[1], &time_cur, sizeof(time_cur));
                write(fd[1], &time_cur, sizeof(time_cur));
                return 0;
            }

            wait(NULL);
            time_t time_cur;
            read(fd[0], &time_cur, sizeof(time_cur));
            struct tm mytime = {};
            localtime_r(&time_cur, &mytime);

            int day = mytime.tm_mday;

            if (day < 10) {
                printf("D:0%d\n", day);
            } else {
                printf("D:%d\n", day);
            }

            return 0;
        }

        wait(NULL);
        time_t time_cur;
        read(fd[0], &time_cur, sizeof(time_cur));
        struct tm mytime = {};
        localtime_r(&time_cur, &mytime);

        int mon = mytime.tm_mon;

        if (mon + 1 < 10) {
            printf("M:0%d\n", mon + 1);
        } else {
            printf("M:%d\n", mon + 1);
        }

        return 0;
    }

    wait(NULL);
    time_t time_cur;
    read(fd[0], &time_cur, sizeof(time_cur));
    struct tm mytime = {};
    localtime_r(&time_cur, &mytime);
    int year = mytime.tm_year;
    printf("Y:%d\n", year + 1900);

    close(fd[0]);
    close(fd[1]);

    return 0;
}