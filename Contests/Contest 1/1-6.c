#include <stdio.h>
#include <stdlib.h>

enum
{
    MIN_N = 1,
    MAX_N = 10000,
    MIN_M = 1,
    MAX_M = 50000
};

int
main(void) 
{
    int n, m;

    if (scanf("%d%d", &n, &m) != 2 || n < MIN_N || n > MAX_N || m < MIN_M || m > MAX_M) {
        exit(1); 
    }

    long long delta;
    unsigned int snow_amount;
    int command_type, left, right;
    unsigned int *snow_data = calloc(n, sizeof(*snow_data));

    if (snow_data == NULL) {
        exit(1);
    }
    
    for (int i = 0; i < m; i++) {
        if (scanf("%d", &command_type) != 1) {
            exit(1);
        }

        if (command_type == 1) {
            if (scanf("%d%d%lld", &left, &right, &delta) != 3) {
                exit(1);
            }

            for (int j = left; j < right; j++) {
                snow_data[j] += delta;
            }
        } else {
            if (scanf("%d%d", &left, &right) != 2) {
                exit(1);
            }

            snow_amount = 0;

            for (int j = left; j < right; j++) {
                snow_amount += snow_data[j];
            }

            printf("%u\n", snow_amount);
        }
    }

    free(snow_data);

    return 0;
}
