#include <stdio.h>
#include <stdlib.h>

typedef char STYPE;
typedef unsigned char UTYPE;

STYPE
bit_reverse(STYPE value)
{
    int delta = 0;
    UTYPE cnt = ~0;
    UTYPE res_u = 0;
    UTYPE value_u = (UTYPE) value;

    while (cnt != 0) {
        cnt = cnt >> 1;
        res_u = res_u << 1;

        if (value_u != 0) {
            delta = value_u & 1;
            res_u += delta;
            value_u = value_u >> 1;
        }
    }

    value = (STYPE) res_u;

    return value;
}

int
main(void)
{
    STYPE a = -13;
    a = bit_reverse(a);

    printf("a = %d\n", a);

    return 0;
}