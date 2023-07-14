#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

int 
main(int argc, const char *argv[])
{
    int cnt = 0;
    
    DIR *d = opendir(argv[1]);
    struct dirent *dd = NULL;
    
    while ((dd = readdir(d)) != NULL) {
        char path[PATH_MAX + 1];
        snprintf(path, PATH_MAX, "%s/%s", argv[1], dd->d_name);
        int len = strlen(path);

        struct stat buf;
        stat(path, &buf);

        if ((S_ISREG(buf.st_mode))) {
            if (len >= 4
                && path[len - 1] == 'e' && path[len - 2] == 'x' && path[len - 3] == 'e' && path[len - 4] == '.') {
                if (access(path, X_OK) == 0) {
                    cnt++;
                }
            }
        }
    }

    printf("%d\n", cnt);
    
    return 0;
}