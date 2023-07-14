#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    SIZE = 4,
    SHIFT_0 = 20,
    SHIFT_1 = 12,
    SHIFT_2 = 8,
};

int
main(int argc, const char *argv[]) 
{
    char buf[SIZE];
    unsigned int x;
    int fd = creat(argv[1], 0600);

    while (scanf("%u", &x) == 1) {
        buf[0] = x >> SHIFT_0;
        buf[1] = x >> SHIFT_1;
        buf[2] = x >> SHIFT_2;
        buf[3] = x;

        buf[0] &= 0x0f;
        buf[2] &= 0x0f;
        
        write(fd, buf, sizeof(buf));
    }

    close(fd);

    return 0;
}