#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <dlfcn.h>

enum { FILE_LENGTH = 100, NUM_SYSTEM = 10 };

int main(int argc, char** argv){

    double left = strtod(argv[1], NULL);
    double right = strtod(argv[2], NULL);
    int n = strtol(argv[3], 0, NUM_SYSTEM);

    int fd = open("removable.c", O_WRONLY | O_TRUNC | O_CREAT, 0660);
    int size = strlen(argv[4]) + FILE_LENGTH;
    char *buf = (char*) calloc (size, sizeof(char));

    snprintf(buf, size, "#include <math.h>\n"
                            "double func(double x) {\n"
                            "return %s;\n"
                            "}", argv[4]);
    dprintf(fd, "%s", buf);
    close(fd);

    double dx = (right - left) / n;
    double ans = 0.0;

    pid_t pid;

    if ((pid = fork()) == 0) {
        execlp("gcc", "gcc", "-shared", "-fPIC", "-Wl,-soname,tmp.so", "-o", "tmp.so", "removable.c", "-lm", NULL);
        _exit(1);
    }

    wait(NULL);

    void *handle = dlopen("./tmp.so", RTLD_LAZY);
    double (*func)(double);
    func = dlsym(handle, "func");

    for (int i = 0; i < n; i++) {
        double x_i = left + i * dx;
        ans += func(x_i) * dx;
    }

    printf("%.10g\n", ans);

    dlclose(handle);
    remove("./removable.c");
    remove("./tmp.so");
    free(buf);
    return 0;
}