//аргументы командной строки: год день день недели

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

enum
{
    DAYS_WEEK = 7,
    SHIFT = 2 * DAYS_WEEK,
    DAYS_NUM = 31,
    MOUNTH_NUM = 12,
    YEAR_START = 1900
};

int 
main(int argc, const char *argv[])
{
    int year;
    scanf("%d", &year);

    for (int month = 0; month < MOUNTH_NUM; month++) {
        int cnt = 0;

        struct tm t = {};
        t.tm_mday = 1;
        t.tm_mon = month;
        t.tm_year = year - YEAR_START;
        t.tm_isdst = -1;
        mktime(&t);

        while (t.tm_wday != 4) {
            t.tm_mday += 1;
            mktime(&t);
        }

        cnt++;
        int day_start = t.tm_mday + DAYS_WEEK;

        for (int day = day_start; day <= DAYS_NUM; day += SHIFT) {
            if ((t.tm_mon == month) && (day % 3 != 0)) {
                printf("%d %d\n", month + 1, day);
            }
        }
    }
    
    return 0;
}