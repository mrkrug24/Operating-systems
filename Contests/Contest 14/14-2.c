#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    unsigned x, alg, max = 1, size = 0;

    while (scanf("%u", &x) == 1) {
        if (x == 0) {
            continue;
        }

        alg = 1;

        if (x % 2 == 0) {
            for (unsigned i = 0; ((x >> i) & 1u) == 0 && i < 32; i++, alg <<= 1);
        }
        
        while (size % alg != 0) {
            size++;
        }

        size += x;

        if (max < alg) {
            max = alg;
        }
    }

    while (size % max != 0) {
        size++;
    }

    if (size == 0 || max == 0) {
        printf("%d %d\n", 1, 1);

    } else {
        printf("%u %u\n", size, max);
    }

    return 0;
}