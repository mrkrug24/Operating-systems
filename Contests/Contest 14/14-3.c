#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int
main(int argc, char *argv[])
{
    unsigned x, size = 0, max = 0;

    while(scanf("%u", &x) != EOF) {
        unsigned tmp = 0;
        unsigned alg = -1;

        while (tmp == 0) {
            alg++;
            tmp = x >> alg;
            tmp = tmp & 1u;
        }

        size += size % (x + alg);

        if (max < alg + (size % (x + alg))) {
            max = alg + (size % (x + alg));
        }

        size += x + alg;
    }

    printf("%u %u\n", size, max);


    return 0;
}