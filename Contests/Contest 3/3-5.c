#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

enum
{
    BITS = 8
};

int
sum_mod(long long x, long mod)
{
    long long sum_sqr = x * (x + 1) * (2 * x + 1) / 6;
    int res = sum_sqr % mod;
    return res;
}

int
main(int argc, const char *argv[])
{
    int fd_1 = open(argv[1], O_RDONLY);
    int fd_2 = open(argv[2], O_TRUNC | O_CREAT | O_WRONLY);

    if (fd_1 == -1 || fd_2 == -1) {
        exit(1);
    }

    errno = 0;
    char *eptr = NULL;
    long mod = strtol(argv[3], &eptr, 10);

    if (errno || *eptr || eptr == argv[3] || (int) mod != mod) {
        exit(1);
    }

    unsigned char x;
    long long num = 1;
    
    while (read(fd_1, &x, sizeof(x)) == sizeof(x)) {
        for (int i = 0; i < BITS; i++) {
            if ((x & 1) == 1) {
                int res = sum_mod(num, mod);
                write(fd_2, &res, sizeof(res));
            }

            x = x >> 1;
            num += 1;
        }
    }

    return 0;
}