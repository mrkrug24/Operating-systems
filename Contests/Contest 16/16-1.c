#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

void update_cache(int *arr, int block_num_addr, int blocks_in_cache,
                    int block_size, unsigned int size, unsigned int addr) {

    arr[block_num_addr % blocks_in_cache] = block_num_addr;

    return;
}


int main(int argc, char** argv) {
    int cache_size = strtol(argv[2], 0, 10);
    int block_size = strtol(argv[3], 0, 10);

    int blocks_in_cache = cache_size / block_size;

    int *arr = calloc(blocks_in_cache, sizeof(*arr));

    for (int i = 0; i < blocks_in_cache; i++) {
        arr[i] = -1;
    }

    char operation[3] = {0};
    unsigned int addr, size;
    int value;
    int block_num_addr;

    int miss_cnt = 0;

    while (1) {
        if (scanf("%c%c", &operation[0], &operation[1]) != 2) {
            break;
        }

        scanf("%x", &addr);
        scanf("%u", &size);
        scanf("%d", &value);

        block_num_addr = addr / block_size;


        if (operation[0] == 'W') {
            update_cache(arr, block_num_addr, blocks_in_cache, block_size, size, addr);

        } else if (operation[0] == 'R') {
            if (arr[block_num_addr % blocks_in_cache] != -1) {
                if (block_num_addr != arr[block_num_addr % blocks_in_cache]) {
                    miss_cnt++;
                }

            }
            update_cache(arr, block_num_addr, blocks_in_cache, block_size, size, addr);
        }
        scanf("%c", &operation[0]);
    }

    free(arr);
    printf("%d\n", miss_cnt);

    return 0;
}