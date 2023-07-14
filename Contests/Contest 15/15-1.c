#include <limits.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char** argv){

    unsigned nums[256];
    unsigned x, max = 0;

    int i = 0;
    while (scanf("%u", &x) == 1) {
        if (i == 0) {
            max = x;
        } else {
            if (x > max) {
                max = x;
            }
        }

        nums[i++] = x;
    }

    for (int k = i - 1; k >= 0; k--) {
        if ((nums[k] != 0) && (max % nums[k] == 0)){
            printf("%u\n", nums[k]);
        }
    }

    return 0;

}