#include <unistd.h>
#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    char *env_var;
    if ((env_var = getenv("XDG_RUNTIME_DIR")) == NULL) {
        if((env_var = getenv("TMPDIR")) == NULL) {
            env_var = "/tmp";
        }
    }

    time_t time_cur;
    time(&time_cur);
    int pid_process = getpid();
    char path_program[PATH_MAX + 1];
    char path_executive[PATH_MAX + 1];
    
    snprintf(path_program, PATH_MAX + 1, "%s/pid_%d_time_%ld_%s.c", 
                env_var, pid_process, time_cur, "expression_value");
    snprintf(path_executive, PATH_MAX + 1, "%s/pid_%d_time_%ld_%s.out",
                    env_var, pid_process, time_cur, "expression_value");

    int fd;
    setbuf(stdin, NULL);
    if ((fd = open(path_program, O_WRONLY | O_TRUNC | O_CREAT, 0660)) == -1) {
        return 1;
    }

    int len = strlen(argv[1]);
    char *program_text = calloc(len + 800, sizeof(*program_text));

    if (program_text == NULL) {
        return 1;
    }

    snprintf(program_text, len + 800,
            "#include <stdio.h>\n\n"
            "int\n"
            "main(int argc, char *argv[]) {\n"
            "   int x;\n"
            "   int reject = 1;\n"
            "   int summon = 2;\n"
            "   int disqualify = 3;\n\n"
            "   while (scanf(\"%%d\", &x) == 1) {\n"
            "       int ans = %s;\n\n"
            "       if (ans == 1) {\n"
            "           printf(\"reject\\n\");\n"
            "       } else if (ans == 2) {\n"
            "           printf(\"summon\\n\");\n"
            "       } else if (ans == 3) {\n"
            "           printf(\"disqualify\\n\");\n"
            "       }\n\n"
            "       fflush(stdout);\n"
            "   }\n\n"
            "   return 0;\n"
            "}", argv[1]);

    dprintf(fd, "%s", program_text);
    close(fd);
    free(program_text);

    pid_t pid = fork();
    
    if (pid > 0) {
        int status;
        wait(&status);
        unlink(path_program);

        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            if ((pid = fork()) > 0) {
                wait(&status);
                unlink(path_executive);

                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    return 0;
                }
                _exit(1);

            } else if (pid == 0) {
                execl(path_executive, path_executive, NULL);
                _exit(1);

            } else {
                fflush(stdout);
                _exit(1);
            }
        }

        _exit(1);

    } else if (pid == 0) {
        execlp("gcc", "gcc", path_program, "-o", path_executive, NULL);
        _exit(1);
    } else {
        fflush(stdout);
        _exit(1);
    }

    return 0;
}