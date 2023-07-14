#include <stdio.h>
#include <stdlib.h>

enum
{
    MY_INT_MAX = ~0u >> !0,
    MY_INT_MIN = ~MY_INT_MAX
};

int satsum(int v1, int v2)
{
    if (v1 < 0 && v2 < 0 && v1 < MY_INT_MIN - v2) {
        return MY_INT_MIN;
    } else if (v1 > 0 && v2 > 0 && v1 > MY_INT_MAX - v2) {
        return MY_INT_MAX;
    }

    return v1 + v2;
}