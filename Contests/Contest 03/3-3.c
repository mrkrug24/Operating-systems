#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

struct Node
{
    int key;
    int left_idx;
    int right_idx;
};

void 
print_keys(int fd, int shift)
{
    lseek(fd, shift, SEEK_SET);

    struct Node node;

    if (read(fd, &node, sizeof(node)) != sizeof(node)) {
        exit(1);
    }

    if (node.right_idx != 0) {
        print_keys(fd, node.right_idx * sizeof(node));
    } 

    printf("%d ", node.key);

    if (node.left_idx != 0) {
        print_keys(fd, node.left_idx * sizeof(node));
    }
}

int
main(int argc, const char *argv[])
{
    int fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        exit(1);
    }

    print_keys(fd, 0);
    printf("\n");

    return 0;
}