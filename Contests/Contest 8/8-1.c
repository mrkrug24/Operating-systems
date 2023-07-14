#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

enum
{
    RADIX = 16,
    OFFSET = 9,
    SIZE = 1 << 16
};

int
main(int argc, char* argv[])
{
    struct stat buf;
    int fd = open(argv[1], O_RDONLY);

    if (fd == -1 || lstat(argv[1], &buf) == -1 || buf.st_size != SIZE) {
        return 1;
    }

    short *phys_mem = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    unsigned short table, addr, phys_addr, phys_page;
    table = strtol(argv[2], 0, RADIX);

    while (scanf("%hx", &addr) == 1) {
        phys_page = phys_mem[table / 2 + (addr >> OFFSET)];
        addr <<= (RADIX - OFFSET);
        addr >>= (RADIX - OFFSET);
        phys_addr = phys_page | addr;

        printf("%hu\n", phys_mem[phys_addr / 2]);
    }

    munmap(phys_mem, SIZE);
    close(fd);

    return 0;
}