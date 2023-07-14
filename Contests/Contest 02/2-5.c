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

int
main(void)
{
    struct Elem *e_1 = calloc(1, sizeof(struct Elem));
    struct Elem *e_2 = calloc(1, sizeof(struct Elem));
    struct Elem *e_3 = calloc(1, sizeof(struct Elem));
    struct Elem *e_4 = calloc(1, sizeof(struct Elem));
    
    e_1->str = "    10";
    e_2->str = "    5x";
    e_3->str = "    alpha";
    e_4->str = "    -03";

    e_1->next = e_2;
    e_2->next = e_3;
    e_3->next = e_4;
    e_4->next = NULL;

    e_1 = dup_elem(e_1);

    while (e_1 != NULL) {
        printf("%s\n", e_1->str);
        e_1 = e_1->next;
    }

    return 0;
}