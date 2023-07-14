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

int
main(int argc, const char *argv[])
{
    int data_input_year;

    if (scanf("%d", &data_input_year) != 1 || data_input_year < 1902 || data_input_year > 2037) {
        return 1;
    }
    
    struct tm data = {};
    data.tm_min = 14;
    data.tm_hour = 11;
    data.tm_mday = 26;
    data.tm_mon = 4;
    data.tm_year = 2021 - 1900;
    data.tm_isdst = -1;
    timegm(&data);

    if (data_input_year < 2021) {
        while (data.tm_year >= data_input_year - 1900) {
            data.tm_min -= 44;
            data.tm_hour -= 12;
            data.tm_mday -= 29;
            timegm(&data);
        }
    }

    while (data.tm_year != data_input_year - 1900 || data.tm_yday < 256) {
        data.tm_min += 44;
        data.tm_hour += 12;
        data.tm_mday += 29;
        timegm(&data);
    }

    int monday_cnt = 0;

    while (monday_cnt != 4) {
        data.tm_mday += 1;
        timegm(&data);

        if (data.tm_wday == 1) {
            monday_cnt += 1;
        }
    }

    if (data.tm_mon + 1 < 10 && data.tm_mday < 10) {
        printf("%d-0%d-0%d\n", data.tm_year + 1900, data.tm_mon + 1, data.tm_mday);
    } else if (data.tm_mon + 1 < 10) {
        printf("%d-0%d-%d\n", data.tm_year + 1900, data.tm_mon + 1, data.tm_mday);
    } else if (data.tm_mday < 10){
        printf("%d-%d-0%d\n", data.tm_year + 1900, data.tm_mon + 1, data.tm_mday);
    } else {
        printf("%d-%d-%d\n", data.tm_year + 1900, data.tm_mon + 1, data.tm_mday);
    }
    
    return 0;
}