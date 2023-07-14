#include <stdio.h>
#include <stdlib.h>

enum
{
    MIN_NUMBER = 0,
    SHIFT_DIG = MIN_NUMBER + 1,
    SHIFT_LOW_LET = SHIFT_DIG + 1 + '9' - '0',
    SHIFT_CAP_LET = SHIFT_LOW_LET + 1 + 'z' - 'a',
    MAX_NUMBER = SHIFT_CAP_LET + 1 + 'Z' - 'A',
    MASK_1 = 0b001000,
    MASK_2 = 0b111011
};

int
convert(int c) {
    if ('0' <= c && c <= '9') {
        c = c - '0' + SHIFT_DIG;
    } else if ('A' <= c && c <= 'Z') {
        c = c - 'A' + SHIFT_CAP_LET;
    } else if ('a' <= c && c <= 'z') {
        c = c - 'a' + SHIFT_LOW_LET;
    } else {
        return 1;
    }
    
    int res;
    c ^= MASK_1;
    c &= MASK_2;

    if (c == MIN_NUMBER) {
        res = '@';
    } else if (SHIFT_DIG <= c && c < SHIFT_LOW_LET) {
        res = '0' + c - SHIFT_DIG;
    } else if (SHIFT_LOW_LET <= c && c < SHIFT_CAP_LET) {
        res = 'a' + c - SHIFT_LOW_LET;
    } else if (SHIFT_CAP_LET <= c && c < MAX_NUMBER) {
        res = 'A' + c - SHIFT_CAP_LET;
    } else {
        res = '#';
    }

    return res;
}

int
main(void)
{
    int c;  
    int res;

    while ((c = getchar()) != EOF) {
        res = convert(c);
        
        if (res != 1) {
            putchar(res);
        }
    }

    return 0;
}
