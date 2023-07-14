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

int 
main(int argc, char *argv[])
{
    DIR *par_1 = opendir(argv[1]);

    if (par_1 == NULL) {
        return 1;
    }

    struct stat buf_1;
    struct stat buf_2;

    char file_1[PATH_MAX] = {};
    char file_2[PATH_MAX] = {};

    long long ans = 0;
    struct dirent *dd_1 = NULL;
    
    while ((dd_1 = readdir(par_1)) != NULL) {
        if ((strcmp(dd_1->d_name, ".") != 0) && (strcmp(dd_1->d_name, "..") != 0)) {
            snprintf(file_1, PATH_MAX, "%s/%s", argv[1], dd_1->d_name);
            
            if (lstat(file_1, &buf_1) != 0) {
                continue;
            }

            if (S_ISREG(buf_1.st_mode) && (access(file_1, W_OK) == 0)) {
                DIR *par_2 = opendir(argv[2]);
                struct dirent *dd_2 = NULL;

                if (par_2 == NULL) {
                    return 1;
                }

                while ((dd_2 = readdir(par_2)) != NULL) {
                    if ((strcmp(dd_2->d_name, ".") != 0) && (strcmp(dd_2->d_name, "..") != 0)) {
                        snprintf(file_2, PATH_MAX, "%s/%s", argv[2], dd_2->d_name);
                        
                        if (stat(file_2, &buf_2) != 0) {
                            continue;
                        }

                        if (buf_1.st_ino == buf_2.st_ino && buf_1.st_dev == buf_2.st_dev) {
                            if (strcmp(dd_1->d_name, dd_2->d_name) == 0) {
                                ans += buf_1.st_size;
                                break;
                            }
                        }
                    }
                }

                closedir(par_2);
            }
        }
    }

    closedir(par_1);
    printf("%lld\n", ans);
    
    return 0;
}