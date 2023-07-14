#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <dirent.h>

enum { HOME_FLAG = 0, TRAVERSE_FLAG = 1 };

struct DirInfo
{
    char* name;
    char* full_path;
};

int comparator(const void *a, const void *b) {
    return strcasecmp(*(const char**)a, *(const char **)b);
}


void directory_traverse(char *name, int flag, char *short_name)
{
    struct dirent *dd;
    struct stat x;

    DIR *start = opendir(name);

    if (start == NULL) {
        return;
    }

    if (flag == TRAVERSE_FLAG) {
        printf("cd %s\n", short_name);
    }

    int ret;
    int cnt = 64;

    struct DirInfo *buf = (struct DirInfo*) malloc (sizeof(*buf) * cnt);

    int size = 0;

    char tmp[PATH_MAX] = {0};

    while ((dd = readdir(start)) != NULL) {
        if ((strcmp(dd->d_name, ".") != 0) && (strcmp(dd->d_name, "..") != 0)) {
            if (snprintf(tmp, PATH_MAX, "%s/%s", name, dd->d_name) >= PATH_MAX) {
                continue;
            }

            ret = lstat(tmp, &x);

            if (ret == -1) {
                continue;
            }

            if (S_ISDIR(x.st_mode)) {
                buf[size].full_path = (char*) malloc (PATH_MAX);
                buf[size].name = (char*) malloc (PATH_MAX);

                snprintf(buf[size].full_path, PATH_MAX, "%s", tmp);
                snprintf(buf[size++].name, PATH_MAX, "%s", dd->d_name);

                if (size >= cnt){
                    cnt *= 2;
                    buf = (struct DirInfo*) realloc (buf, sizeof(struct DirInfo) * cnt);
                }
            }
        }
    }

    closedir(start);

    qsort(buf, size, sizeof(*buf), comparator);

    for (int i = 0; i < size; i++) {
        directory_traverse(buf[i].full_path, TRAVERSE_FLAG, buf[i].name);
    }

    for (int i = 0; i < size; i++) {
        free(buf[i].full_path);
        free(buf[i].name);
    }
    free(buf);

    if (flag == TRAVERSE_FLAG) {
        printf("cd ..\n");
    }

    return;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        return 1;
    }

    directory_traverse(argv[1], HOME_FLAG, NULL);

    return 0;
}