#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned long long
alignment(unsigned long long x) {
    if (x % 2 == 1) {
        return 1;
    }

    unsigned long long tmp_1 = 0, tmp_2 = x, alg = 1;

    while (tmp_1 == 0) {
        tmp_2 >>= 1;
        tmp_1 = tmp_2;
        tmp_1 = tmp_1 & 1u;
        alg <<= 1;
    }

    return alg;
}

int
main(int argc, char *argv[])
{
    unsigned long long x;
    scanf("%llu", &x);

    printf("%llu\n", alignment(x));

    return 0;
}