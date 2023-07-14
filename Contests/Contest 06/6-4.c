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

enum
{
    BITS_NUM = 8
};

int
main(int argc, char *argv[])
{
    struct stat buf;
    int fd = open(argv[1], O_RDWR);

    if (stat(argv[1], &buf) == -1 || fd == -1) {
        close(fd);
        return 1;
    }

    long x;
    long size = buf.st_size;

    while (scanf("%ld", &x) == 1) {
        if (x > BITS_NUM * size || x < BITS_NUM * size * (-1) || x == 0) {
            continue;
        }

        unsigned char x_byte, insert = 1u;
        long reverse = (x < 0) ? (x = -x, 1) : 0;
        long bit_num = (x - 1) % BITS_NUM;
        long byte_num = (x - 1) / BITS_NUM;
        insert = insert << bit_num;
        
        if (lseek(fd, byte_num, SEEK_SET) == -1 || read(fd, &x_byte, sizeof(x_byte)) == -1) {
            close(fd);
            return 1;
        }

        (reverse) ? (insert = ~insert, x_byte &= insert) : (x_byte |= insert);

        if (lseek(fd, -1, SEEK_CUR) == -1 || write(fd, &x_byte, sizeof(x_byte)) == -1) {
            close(fd);
            return 1;
        }
    }

    close(fd);
    return 0;
}