#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>

enum
{
    BUF_SIZE = 4096
};

int
copy_file(const char *srcpath, const char *dstpath)
{
    int fd_dst;
    int fd_src = open(srcpath, O_RDONLY);
    struct stat buf_src;
    struct stat buf_dst;

    if ((stat(srcpath, &buf_src) == -1) || S_ISDIR(buf_src.st_mode) || (fd_src == -1)) {
        return -1;
    }

    if ((stat(dstpath, &buf_dst) == 0) && (buf_src.st_ino == buf_dst.st_ino) && (buf_src.st_dev == buf_dst.st_dev)) {
        return 0;
    }

    if ((dstpath == NULL) || (dstpath[0] == '\0')) {
        return -1;
    }

    if ((strlen(dstpath) == 1) && (*dstpath == '/')) {
        return -1;
    }

    int cur = 0;
    int note = 0;
    char inf[BUF_SIZE];
    char *inf_p = inf;
    
    if ((stat(dstpath, &buf_dst) == -1) || !(S_ISDIR(buf_dst.st_mode))) {
        fd_dst = creat(dstpath, buf_src.st_mode);

        while ((cur = read(fd_src, inf, sizeof(inf))) > 0) {
            inf_p = inf;

            while (cur > 0) {
                note = write(fd_dst, inf_p, cur);
                inf_p += note;
                cur -= note;
            }
        }
    } else {
        int cnt = 0;
        int len = strlen(srcpath);
        int i = len - 1;

        for (int j = len - 1; j >= 0; j--) {
            if (srcpath[j] == '/')  {
                break;
            }

            cnt++;
            i--;
        }

        int full_len = strlen(dstpath) + cnt + 2;
        char full_name[full_len];

        for (int j = 0; j < strlen(dstpath); j++) {
            full_name[j] = dstpath[j];
        }

        full_name[strlen(dstpath)] = '/';

        for (int j = 0; j < cnt; j++) {
            full_name[strlen(dstpath) + j + 1] = srcpath[i + j + 1];
        }

        full_name[full_len - 1] = '\0';

        if ((stat(full_name, &buf_dst) == 0)
            && (buf_src.st_ino == buf_dst.st_ino) && (buf_src.st_dev == buf_dst.st_dev)) {
            return 0;
        }

        fd_dst = creat(full_name, buf_src.st_mode);

        while ((cur = read(fd_src, inf, sizeof(inf))) > 0) {
            inf_p = inf;
            
            while (cur > 0) {
                note = write(fd_dst, inf_p, cur);
                inf_p += note;
                cur -= note;
            }
        }
    }

    close(fd_src);
    close(fd_dst);
    
    return 0;
}