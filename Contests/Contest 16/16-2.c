#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>

enum { CONST = 8 };

int64_t min(int64_t a, int64_t b) {
    return ((a < b) ? a : b);
}

int64_t calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count) {
    int flag_disk_size = 0, flag_fs5_size = 0;
    int64_t disk_size = 0, fs5_size = 0;

    if (block_num_size * CONST < (sizeof(int64_t) * CHAR_BIT)) {
        disk_size = 1llu << (block_num_size * CONST);
        if (__builtin_smulll_overflow(disk_size, block_size, &disk_size)) {
            flag_disk_size = 1;
        }
    } else {
        flag_disk_size = 1;
    }


    int64_t tmp = block_size / block_num_size;
    int64_t tmp_copy = tmp;
    fs5_size = inode_direct_block_count;

    if (__builtin_saddll_overflow(fs5_size, tmp, &fs5_size)) {
        flag_fs5_size = 1;
    }
    if (__builtin_smulll_overflow(tmp, tmp, &tmp)) {
        flag_fs5_size = 1;
    }
    if (__builtin_saddll_overflow(fs5_size, tmp, &fs5_size)) {
        flag_fs5_size = 1;
    }
    if (__builtin_smulll_overflow(tmp, tmp_copy, &tmp)) {
        flag_fs5_size = 1;
    }
    if (__builtin_saddll_overflow(fs5_size, tmp, &fs5_size)) {
        flag_fs5_size = 1;
    }
    if (__builtin_smulll_overflow(fs5_size, block_size, &fs5_size)) {
        flag_fs5_size = 1;
    }

    if ((flag_disk_size == 1) && (flag_fs5_size == 0)) {
        return fs5_size;
    }

    if ((flag_disk_size == 0) && (flag_fs5_size == 1)) {
        return disk_size;
    }

    if ((flag_disk_size == 1) && (flag_fs5_size == 1)) {
        return -1;
    } else {
        return min(disk_size, fs5_size);
    }
}

int main(int argc, char** argv) {
    int64_t block_size, block_num_size, inode_direct_block_count;

    scanf("%" SCNd64 "%" SCNd64 "%" SCNd64, &block_size, &block_num_size, &inode_direct_block_count);

    int64_t tmp = calc_max_size(block_size, block_num_size, inode_direct_block_count);

    printf("%" PRId64 "\n", tmp);

    int64_t i = 1;
    int64_t max_i = 0;
    int64_t max_res = 0;

    for (i = 1; i <= block_size; i++) {
        tmp = calc_max_size(block_size, i, inode_direct_block_count);
        if (tmp == -1) {
            printf("%" PRId64 " %" PRId64 "\n", i, tmp);
            return 0;
        } else if (tmp > max_res) {
            max_res = tmp;
            max_i = i;
        }
    }

    printf("%" PRId64 " %" PRId64 "\n", max_i, max_res);

    return 0;
}