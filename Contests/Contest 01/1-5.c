#include <stdio.h>
#include <stdlib.h>

enum
{
    MIN_VALUE = 0,
    MAX_VALUE = 10
};

void
list_of_perm(int n, int *perm, int shift, int *shift_flags)
{
    if (shift == n) {
        for (int i = 0; i < n; i++) {
            printf("%d", perm[i]);
        }
        
        printf("\n");

    } else {
        for (int i = 0; i < n; i++) {
            if (shift_flags[i] == 0) {
                perm[shift] = i + 1;
                shift_flags[i] = 1;

                list_of_perm(n, perm, shift + 1, shift_flags);

                shift_flags[i] = 0;
            }
        }
    }
}

int
main(void) 
{
    int n;
    if (scanf("%d", &n) != 1 || n <= MIN_VALUE || n >= MAX_VALUE) {
        exit(1);
    }

    int *perm = calloc(n, sizeof(*perm));
    int *shift_flags = calloc(n, sizeof(*shift_flags));

    if (perm == NULL || shift_flags == NULL) {
        exit(1);
    }

    list_of_perm(n, perm, 0, shift_flags);
    free(perm);
    free(shift_flags);

    return 0;
}