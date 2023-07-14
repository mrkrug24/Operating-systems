#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

enum
{
    BITS = 9
};

const char sample[] = "rwxrwxrwx";

int 
main(int argc, const char *argv[])
{
    char *ans = calloc(BITS + 1, sizeof(ans));

    for (int i = 1; i < argc; i++) {
        errno = 0;
        char *eptr = NULL;
        long x = strtol(argv[i], &eptr, 8);

        if (errno || *eptr || eptr == argv[i] || (int) x != x) {
            exit(1);
        }

        for (int j = BITS - 1; j >= 0; j--) {
            if ((x & 1) == 0) {
                ans[j] = '-';
            } else {
                ans[j] = sample[j];
            }

            x = x >> 1;
        }

        printf("%s\n", ans);
    }

    free(ans);
    
    return 0;
}