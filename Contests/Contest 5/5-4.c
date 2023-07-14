#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

enum
{
    LEVEL_MAX = 4,
    START_NUM = 8
};

void
print_path(char *prev_name, long size, int level, int len) 
{
    DIR *par = opendir(prev_name);

    if (par == NULL || prev_name == NULL) {
        return;
    }

    struct stat buf;
    struct dirent *dd = NULL;

    int nmb = 0;
    int cnt = START_NUM;
    char **list = calloc(cnt, sizeof(*list));

    if (list == NULL) {
        return;
    }

    while ((dd = readdir(par)) != NULL) {
        if ((strcmp(dd->d_name, ".") != 0) && (strcmp(dd->d_name, "..") != 0)) {
            char *next = calloc(PATH_MAX, sizeof(*next));

            if (next == NULL) {
                return;
            }

            snprintf(next, PATH_MAX, "%s/%s", prev_name, dd->d_name);
            
            if (lstat(next, &buf) != 0) {
                free(next);
                continue;
            }

            if (S_ISREG(buf.st_mode) && (buf.st_size <= size) && (access(next, R_OK) == 0)) {
                printf("%s\n", next + len + 1);
                free(next);
            } else if (S_ISDIR(buf.st_mode) && level < LEVEL_MAX) {
                if (nmb >= cnt - 1) {
                    cnt = cnt * 2;

                    if ((list = realloc(list, sizeof(*list) * cnt)) == NULL) {
                        return;
                    }
                }

                list[nmb++] = next;
            } else {
                free(next);
            }
        }
    }

    closedir(par);

    for (int i = 0; i < nmb; i++) {
        print_path(list[i], size, level + 1, len);
        free(list[i]);
    }

    free(list);
}

int 
main(int argc, char *argv[])
{
    errno = 0;
    char *eptr = NULL;
    long size = strtol(argv[2], &eptr, 10);

    if (errno || *eptr || eptr == argv[2] || (long) size != size) {
        exit(1);
    }

    print_path(argv[1], size, 1, strlen(argv[1]));

    return 0;
}