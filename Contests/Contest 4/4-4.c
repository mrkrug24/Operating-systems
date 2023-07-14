#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>

int
cmp(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b); 
}

int 
main(int argc, char **argv)
{
    int cnt = 0;
    struct stat buf;
    char **path = calloc(argc - 1, sizeof(*path));
    long *ino = calloc(argc - 1, sizeof(*ino));
    long *dev = calloc(argc - 1, sizeof(*dev));

    if (path == NULL || ino == NULL || dev == NULL) {
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (stat(argv[i], &buf) == 0) {
            int into = 0;

            for (int j = 0; j < cnt; j++) {
                if (ino[j] == buf.st_ino && dev[j] == buf.st_dev) {
                    if (strcmp(argv[i], path[j]) > 0) {
                        path[j] = argv[i];
                    }

                    into = 1;

                    break;
                }
            }

            if (into == 0) {
                path[cnt] = argv[i];
                ino[cnt] = buf.st_ino;
                dev[cnt] = buf.st_dev;

                cnt += 1;
            }
        }
    }

    qsort(path, cnt, sizeof(*path), cmp);

    for (int i = 0; i < cnt; i++) {
        printf("%s\n", path[i]);
    }

    free(path);
    free(ino);
    free(dev);
    
    return 0;
}