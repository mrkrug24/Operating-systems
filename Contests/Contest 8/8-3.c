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
#include <time.h>
#include <sys/mman.h>

int 
main(int argc, char **argv)
{
    long rows, cols;

    errno = 0;
    char *eptr = NULL;
    rows = strtol(argv[2], &eptr, 10);

    if (errno || *eptr || eptr == argv[2] || (long) rows != rows) {
        exit(1);
    }

    errno = 0;
    eptr = NULL;
    cols = strtol(argv[3], &eptr, 10);

    if (errno || *eptr || eptr == argv[3] || (long) cols != cols) {
        exit(1);
    }

    int fd_input = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0777);

    if (fd_input == -1) {
        return 1;
    }

    ftruncate(fd_input, rows * cols * 4);

    if (rows == 0 || cols == 0) {
        close(fd_input);
        return 0;
    }

    int *input = mmap(NULL, rows * cols * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd_input, 0);

    if (input == MAP_FAILED) {
        return 1;
    }
    
    int i = 0;
    int j = 0;
    int cnt = 1;

    int first_row = 0;
    int first_col = 0;
    int last_row = rows - 1;
    int last_col = cols - 1;

    while (cnt <= rows * cols) {
        input[i * cols + j] = cnt;
        cnt += 1;

        if (i == first_row) {
            if (j < last_col) {
                j += 1;
            } else if (i < last_row) {
                i += 1;
            }

        } else if (j == last_col) {
            if (i < last_row) {
                i += 1;
            } else if (j > first_col) {
                j -= 1;
            }

        } else if (i == last_row) {
            if (j > first_col) {
                j -= 1;
            } else if (i > first_row) {
                i -= 1;
            }


        } else if (j == first_col) {
            if (i > first_row + 1) {
                i -= 1;
            } else {
                j += 1;
                first_row += 1;
                first_col += 1;
                last_row -= 1;
                last_col -= 1;
            }
        }        
    }

    munmap(input, rows * cols * 4);
    close(fd_input);
 
    return 0;
}