#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum
{
    SIZE = 64
};

typedef struct Args
{
    char s[SIZE];
} Args;

int
main (int argc, char *argv[])
{
    void *library;
    if ((library = dlopen(argv[1], RTLD_LAZY)) == NULL) {
        return 1;
    }

    void *function;
    if ((function = dlsym(library, argv[2])) == NULL) {
        return 1;
    }

    int i; 
    double d;
    char *s;
    int shift = 0;
    int delta = 0;
    
    Args *arguments = calloc(1, sizeof(*arguments));

    if (arguments == NULL) {
        return 1;
    }
    
    for (int k = 4; k < argc; k++) {
        if (argv[3][k - 3] == 'i') {
            delta = sizeof(i);
            i = strtol(argv[k], 0, 10);
            memcpy(&(arguments->s[shift]), &i, sizeof(i));

        } else if (argv[3][k - 3] == 'd') {
            delta = sizeof(d);
            d = strtod(argv[k], 0);
            memcpy(&(arguments->s[shift]), &d, sizeof(d));

        } else if (argv[3][k - 3] == 's') {
            delta = sizeof(s);
            s = argv[k];
            memcpy(&(arguments->s[shift]), &s, sizeof(s));
        }

        shift += delta;
    }

    if (argv[3][0] == 'v') {
        ((void(*)(Args))(function))(*arguments);

    } else if (argv[3][0] == 'i') {
        printf("%d\n", ((int(*)(Args))(function))(*arguments));

    } else if (argv[3][0] == 'd') {
        printf("%.10g\n", ((double(*)(Args))(function))(*arguments));
        
    } else if (argv[3][0] == 's') {
        printf("%s\n", ((char *(*)(Args))(function))(*arguments));
    }

    free(arguments);

    if (dlclose(library) != 0) {
        return 1;
    }
    
    return 0;
}