#include <stdio.h>
#include <stdlib.h>

enum
{
    MIN_VALUE = 1,
    MAX_VALUE = 2000
};

int
main(void)
{
    int n;
    
    if (scanf("%d", &n) != 1 || n <= MIN_VALUE || n >= MAX_VALUE) {
        exit(1);
    }

    int *first_string = calloc(n - 1, sizeof(*first_string));
    int *next_string = calloc(n - 1, sizeof(*next_string));

    if (first_string == NULL || next_string == NULL) {
        exit(1);
    }

    for (int i = 0; i < n - 1; i++) {
        printf("%d ", first_string[i]);
    }

    printf("\n");

    for (int i = 0; i < n - 1; i++) {
        for (int j = 1; j < n; j++) {
            if (((i + 1) * j) % n == 1) {
                first_string[i] = j;
                break;
            }
        }
    }

    for (int i = 0; i < n - 1; i++) {
        printf("%d ", first_string[i]);
    }

    printf("\n");

    if (n != 2) {
        for (int i = 2; i < n; i++) {
            for (int j = 0; j < n - 1; j++) {
                next_string[j] = (first_string[j] * i) % n;
            }

            for (int i = 0; i < n - 1; i++) {
                printf("%d ", next_string[i]);
            }

            printf("\n");
        }
    }

    free(first_string);
    free(next_string);

    return 0;
}