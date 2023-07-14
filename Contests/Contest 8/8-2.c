#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

enum
{
    MULTIPLIER = 1103515245,
    INCREMENT = 12345,
    MODULE = 1u << 31
};

typedef struct RandomGenerator
{
    int a, c;
    unsigned int value, m;
    const struct RandomOperations* ops;
} RandomGenerator;

void
destroy(RandomGenerator *rr)
{
    (rr != NULL) ? (free(rr)) : 0;
    return;
}

unsigned int
next (RandomGenerator *rr)
{
    unsigned int res;
    (rr != NULL) ? (rr->value = ((rr->a) * (rr->value) + (rr->c)) % (rr->m), res = rr->value) : (res = 0);
    return res;
}

static const struct RandomOperations 
{
    void(*destroy)(RandomGenerator *rr);
    unsigned int(*next)(RandomGenerator *rr);
} ops = {destroy, next}; 

RandomGenerator 
*random_create(int seed)
{
    RandomGenerator *rr = calloc(1, sizeof(*rr));

    if (rr != NULL) {
        rr->a = MULTIPLIER;
        rr->c = INCREMENT;
        rr->m = MODULE;
        rr->value = seed;
        rr->ops = &ops;
    }

    return rr;
}