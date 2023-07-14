#include <stdio.h>
#include <stdlib.h>

static const double X_MIN = 2.0;
static const double X_MAX = 5.0;
static const double Y_MIN = 1.0;
static const double Y_MAX = 7.0;
static const double X_SHIFT = 2.0;

int
main(void)
{
    double x, y;
    
    if (scanf("%lf%lf", &x, &y) == 2) {
        int res = (x >= X_MIN && x <= X_MAX && y >= Y_MIN && y <= Y_MAX && y >= x - X_SHIFT);
        printf("%d\n", res);
    } else {
        exit(1);
    }

    return 0;
}