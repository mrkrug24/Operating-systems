#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

enum
{
    DATA_LEN = 150
};

int main(int argc, char *argv[])
{
    char *env_var;
    if ((env_var = getenv("XDG_RUNTIME_DIR")) == NULL) {
        if((env_var = getenv("TMPDIR")) == NULL) {
            env_var = "/tmp";
        }
    }

    time_t time_cur;
    time(&time_cur);
    int pid = getpid();
    char path[PATH_MAX + 1];
    snprintf(path, PATH_MAX + 1, "%s/pid_%d_time_%ld_%s.py", env_var, pid, time_cur, "python_multiply");

    int fd;
    if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0770)) == -1) {
        return 1;
    }

    int len = 0, cur = 0;
    for (int i = 1; i < argc; i++) {
        len += strlen(argv[i]);
    }

    char *comand_text = calloc(len + argc + 1, sizeof(*comand_text));
    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < strlen(argv[i]); j++) {
            comand_text[cur++] = argv[i][j];
        }

        if (i != argc - 1) {
            comand_text[cur++] = '*';
        } else {
            comand_text[cur] = '\0';
        }
    }

    char *program_text = calloc(len + argc + DATA_LEN, sizeof(*program_text));
    snprintf(program_text, len + argc + DATA_LEN,
            "#!/usr/bin/python3\n"
            "import os\n"
            "import sys\n"
            "sys.set_int_max_str_digits(10**6)\n"
            "print(%s)\n"
            "os.remove(__file__)\n",
            comand_text);

    dprintf(fd, "%s", program_text);
    close(fd);
    free(comand_text);
    free(program_text);
    execlp(path, path, NULL);

    return 0;
}