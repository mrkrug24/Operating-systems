#include <stdio.h>
#include <stdlib.h>

int
cmp(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

    if (x % 2 == 0 && y % 2 == 0) {
        if (x < y) {
            return -1;
        } else if (x == y) {
            return 0;
        } else {
            return 1;
        }
    } else if (x % 2 != 0 && y % 2 != 0) {
        if (x > y) {
            return -1;
        } else if (x == y) {
            return 0;
        } else {
            return 1;
        }
    } else if (x % 2 == 0 && y % 2 != 0) {
        return -1;
    } else {
        return 1;
    }
}

void 
sort_even_odd(size_t count, int *data) 
{
    qsort(data, count, sizeof(*data), cmp);
}