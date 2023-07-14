#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int 
main(int argc, const char **argv)
{
    int x;
    long long sum_1 = 0;
    long long sum_2 = 0;
 
    for (int i = 1; i < argc; i++) {
        errno = 0;
        char *eptr = NULL;
        x = strtol(argv[i], &eptr, 10);

        if (errno || *eptr || eptr == argv[i] || (int) x != x) {
            exit(1);
        }

        if (x > 0 && __builtin_add_overflow(sum_1, x, &sum_1) == 1) {
            exit(1);
        }

        if (x < 0 && __builtin_add_overflow(sum_2, x, &sum_2) == 1){
            exit(1);
        }
    }

    printf("%lld\n%lld\n", sum_1, sum_2);

    return 0;
}