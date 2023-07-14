#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

enum
{
    DEGREE = 10000
};

int
main(int argc, const char **argv)
{
    errno = 0;
    double delta;
    char *eptr = NULL;
    double value = strtod(argv[1], &eptr);

    if (errno || *eptr || eptr == argv[1] || (double) value != value) {
        exit(1);
    }

    for (int i = 2; i < argc; i++) {
        errno = 0;
        char *eptr = NULL;
        delta = strtod(argv[i], &eptr);

        if (errno || *eptr || eptr == argv[1] || (double) delta != delta) {
            exit(1);
        }

        delta = (100 + delta) / 100;
        value *= delta;
        value *= DEGREE;
        value = round(value);
        value /= DEGREE;
    }

    printf("%.4lf\n", value);
    
    return 0;
}