#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

enum
{
    DEGREE = 3
};

int 
main(int argc, char *argv[])
{
    char cur = '\0';

    while ((cur = getchar()) != EOF) {
        if (cur != '0' && cur != '1' && cur != 'a') {
            continue;

        } else {
            long long ans = 0;
            int overflow = 0;

            while (cur == '0' || cur == '1' || cur == 'a') {
                if (__builtin_mul_overflow(ans, DEGREE, &ans) == 1) {
                    overflow = 1;
                }

                if ((cur == '1') && (__builtin_add_overflow(ans, 1, &ans) == 1)) {
                    overflow = 1;
                }

                if ((cur == 'a') && (__builtin_add_overflow(ans, -1, &ans) == 1)) {
                    overflow = 1;
                }

                if (ans == (LLONG_MIN / DEGREE) - 1) {
                    cur = getchar();

                    if (cur == '1') {
                        cur = getchar();

                        if (cur != '0' && cur != '1' && cur != 'a') {
                            ans = LLONG_MIN;
                            break;

                        } else {
                            overflow = 1;
                        }

                    } else if (cur == '0' || cur == 'a') {
                        overflow = 1;
                    } else {
                        continue;
                    }
                }

                cur = getchar();
            }

            if (overflow == 0) {
                printf("%lld\n", ans);
            } else {
                printf("18446744073709551616\n");
            }
        }
    }

    return 0;
}