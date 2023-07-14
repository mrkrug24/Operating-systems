#include <limits.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

unsigned long long int convert(unsigned char buf[]) {
    unsigned long long int num = 0;

    num |= buf[0];
    num <<= CHAR_BIT;
    num |= buf[1];
    num <<= CHAR_BIT;
    num |= buf[2];
    num <<= CHAR_BIT;
    num |= buf[3];
    num <<= CHAR_BIT;
    num |= buf[4];
    num <<= CHAR_BIT;
    num |= buf[5];
    num <<= CHAR_BIT;
    num |= buf[6];
    num <<= CHAR_BIT;
    num |= buf[7];

    return num;
}

int main(int argc, char** argv){

    int fd = open(argv[1], O_RDONLY);
    int tmp = open("./tmp.bin", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    unsigned long long int num = strtoll(argv[2], 0, 10);

    unsigned char buf[8] = {0};
    unsigned long long int x;

    while (read(fd, buf, sizeof(buf)) == sizeof(buf)) {
        x = convert(buf);
        if (x >= num) {
            write(tmp, &x, sizeof(x));
        }
    }

    close(fd);
    close(tmp);

    tmp = open("./tmp.bin", O_RDONLY);
    fd =  open(argv[1], O_WRONLY | O_TRUNC);

    while (read(tmp, buf, sizeof(buf)) == sizeof(buf)) {
        x = convert(buf);
        write(fd, &x, sizeof(x));
    }

    close(fd);
    close(tmp);

    return 0;
}