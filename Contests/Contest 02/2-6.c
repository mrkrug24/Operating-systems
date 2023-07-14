#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

enum
{
    LEN = 12,
    MY_INT_MAX = ~0u >> !0
};

struct Elem
{
    struct Elem *next;
    char *str;
};

struct Elem
*dup_elem(struct Elem *head)
{
    long num;
    struct Elem *cur = head;
    struct Elem *prev = NULL;

    while (cur != NULL) {
        errno = 0;
        char *eptr = NULL;
        num = strtol(cur->str, &eptr, 10);

        if (!(errno) && !(*eptr) && eptr != cur->str && (int) num == num && num < MY_INT_MAX) {
            struct Elem *new_elem = calloc(1, sizeof(*new_elem));
            char *s = calloc(LEN, sizeof(*s));
            snprintf(s, LEN, "%ld", num + 1);
            
            new_elem->str = s;
            new_elem->next = cur;

            if (prev == NULL) {
                head = new_elem;
            } else {
                prev->next = new_elem;
            }
        }

        prev = cur;
        cur = cur->next;
    }

    return head;
}