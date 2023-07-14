#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

enum
{
    DEG = 10,
    THS = 1000
};

int
main(int argc, char *argv[])
{
    long long ans = 0;

    for (int i = 1; i < argc; i++) {
        errno = 0;
        char *eptr = NULL;
        long num = strtol(argv[i], &eptr, DEG);
    
        if (errno || (int) num != num) {
            continue;
        }

        if (*eptr == '-') {
            ans -= num;

        } else if (*eptr == '+') {
            ans += num;

        } else  if (*eptr == 'k') {
            if (__builtin_mul_overflow(THS, num, &num) == 1) {
                continue;
            }

            if (*(eptr + 1) == '-') {
                ans -= num;

            } else if (*(eptr + 1) == '+') {
                ans += num;
            }
        }
    }

    printf("%lld\n", ans);

    return 0;
}