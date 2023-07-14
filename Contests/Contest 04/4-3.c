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
#include <string.h>

const char sample[] = "rwxrwxrwx";

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL) {
        return -1;
    }

    int ans = 0;

    int len = sizeof(sample) / sizeof(sample[0]) - 1;

    if (strlen(str) != len) {
        return -1;
    }

    for (int i = 0; i < len; i++) {
        ans = ans << 1;

        if (str[i] == sample[i]) {
            ans += 1;
        } else if (str[i] != '-') {
            return -1;
        }
    }

    return ans;
}