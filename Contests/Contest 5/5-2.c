#include <stdio.h>
#include <stdlib.h>

void
normalize_path(char *buf) {
    if (buf == NULL) {
        exit(1);
    }

    int i = 0;

    while (buf[i] != '\0') {
        if (buf[i] == '/' && buf[i + 1] != '\0' && buf[i + 1] == '.' 
            && (buf[i + 2] == '/' || buf[i + 2] == '\0')) {

            if (buf[i + 2] == '/') {
                int j = i + 1;

                while (buf[j + 2] != '\0') {
                    buf[j] = buf[j + 2];
                    j += 1;
                }

                buf[j] = '\0';
            } else {
                buf[i] = '\0';
            }

        } else if (buf[i] == '/' && buf[i + 1] != '\0' && buf[i + 1] == '.' 
            && buf[i + 2] != '\0' && buf[i + 2] == '.' 
            && (buf[i + 3] == '/' || buf[i + 3] == '\0')) {
            int prev = i - 1;
            char tmp = buf[i + 3];

            while (prev > 0 && buf[prev] != '/') {
                prev -= 1;
            }

            if (prev < 0) {
                prev = 0;
            }

            int back = prev;

            if (tmp == '/') {
                prev += 1;
                int j = i + 4;
                int shift = j - prev;

                while (buf[prev + shift] != '\0') {
                    buf[prev] = buf[prev + shift];
                    prev += 1;
                }

                buf[prev] = '\0';
                i = back;
            } else {
                buf[prev] = '\0';
                i = prev;
            }
        } else {
            i += 1;
        }
    }

    if (i == 0 && buf[i] == '\0') {
        buf[i] = '/';
        buf[i + 1] = '\0';
    } else if (i > 1 && buf[i - 1] == '/') {
        buf[i - 1] = '\0';
    }
}

int main(void) {
    char s[] = "/a/b/c/../../../cd/d/../../../..";
    normalize_path(s);
    puts(s);

}