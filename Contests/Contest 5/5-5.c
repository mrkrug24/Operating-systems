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

ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    DIR *work_dir = opendir(".");

    if (work_dir == NULL) {
        return -1;
    }

    int fd_work = dirfd(work_dir);

    if (fd_work == -1) {
        return -1;
    } 

    if (fd == -1 || buf == NULL) {
        return -1;
    }
    
    if (fchdir(fd) != 0) {
        return -1;
    }

    struct stat root;
    struct stat cur;
    struct stat prev;

    if (lstat(".", &prev) != 0 || lstat("/", &root) != 0) {
        return -1;
    }

    if (prev.st_ino == root.st_ino && prev.st_dev == root.st_dev) {
        if (size == 1) {
            buf[0] = '\0';
        } else if (size > 1) {
            buf[0] = '/';
            buf[1] = '\0';
        }

        fchdir(fd_work);
        closedir(work_dir);

        return 1;
    }

    if (chdir("..") != 0) {
        return -1;
    }

    struct dirent *dd = NULL;
    char path[PATH_MAX + 1] = {};
    char full_path[PATH_MAX + 1] = {};

    while (prev.st_ino != root.st_ino || prev.st_dev != root.st_dev) {
        DIR *par = opendir(".");

        if (par == NULL) {
            return -1;
        }

        while ((dd = readdir(par)) != NULL) {
            snprintf(path, PATH_MAX + 1, "./%s", dd->d_name);

            if (lstat(path, &cur) != 0) {
                continue;
            }

            if (prev.st_ino == cur.st_ino && prev.st_dev == cur.st_dev) {
                memcpy(path, full_path, PATH_MAX + 1);
                snprintf(full_path, PATH_MAX + 1, "/%s%s", dd->d_name, path);
                break;
            }
        }
        
        closedir(par);

        if (lstat(".", &prev) != 0 || chdir("..") != 0) {
            return -1;
        }
    }

    if (size > 0) {
        int len = strlen(full_path);

        if (len < size) {
            memcpy(buf, full_path, len);
            buf[len] = '\0';
        } else {
            memcpy(buf, full_path, size - 1);
            buf[size - 1] = '\0';
        }
    }

    fchdir(fd_work);
    closedir(work_dir);

    return strlen(full_path);
}


int
main(int argc, const char *argv[])
{
    char path[] = "/mnt/d/Documents/Programming/C/Contests";

    DIR *file = opendir(path);
    int size = 10;
    char *buf = calloc(size, sizeof(*buf));
    int fd = dirfd(file);

    long long len = getcwd2(fd, buf, size);
    printf("%lld\n", len);
    closedir(file);

    puts(buf);

    return 0;
}