#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    int n, x;
    scanf("%d\n", &n);

    int *arr = calloc(n - 4, sizeof(*arr));

    for (int i = 2; i < n - 2; i++) {
        scanf("%d\n", &arr[i]);
    }

    while (scanf("%d\n", &x) != EOF) {
        int cnt = 0;
        while (x != 0 && x != n - 1 && x != n - 2 && x != 1) {
            x = arr[x];
            cnt++;
        }

        printf("%d\n", cnt);
    }

    free(arr);

    return 0;
}