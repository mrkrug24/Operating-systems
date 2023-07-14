#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

int
main(int argc, const char *argv[])
{
    int fd = open(argv[1], O_RDWR);

    if (fd == -1) {
        exit(1);
    }

    long long cur;

    if (read(fd, &cur, sizeof(cur)) != sizeof(cur)) {
        close(fd);
        return 0;
    }

    long long min = cur;
    long long ind_cur = 0;
    long long ind_min = 0;
    
    while (read(fd, &cur, sizeof(cur)) == sizeof(cur)) {
        ind_cur += sizeof(cur);

        if (cur < min) {
            min = cur;
            ind_min = ind_cur;
        }
    }

    long long llong_min = (unsigned long long)1 << (8 * sizeof(llong_min) - 1);

    if (min != llong_min) {
        long long res = -min;
        lseek(fd, ind_min, SEEK_SET);
        write(fd, &res, sizeof(res));
    }

    close(fd);

    return 0;
}