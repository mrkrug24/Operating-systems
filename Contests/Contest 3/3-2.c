#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

int
main(int argc, const char *argv[]) 
{
    int fd = open(argv[1], O_RDWR);

    if (fd == -1) {
        exit(1);
    }

    errno = 0;
    char *eptr = NULL;
    long n = strtol(argv[2], &eptr, 10);

    if (errno || *eptr || eptr == argv[2] || (int) n != n) {
        exit(1);
    }

    if (n < 1) {
        return 0;
    }

    double x = 0;
    int size = sizeof(x);
    
    if (read(fd, &x, size) != size) {
        return 0;
    }

    int i = 1;
    double prev = x;
    
    while (read(fd, &x, size) == size && i < n) {
        i++;
        x -= prev;
        prev = x;
        lseek(fd, -size, SEEK_CUR);
        write(fd, &x, size);
    }

    close(fd);

    return 0;
}