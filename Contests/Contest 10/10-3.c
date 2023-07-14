#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/wait.h>

int
main(int argc, const char *argv[])
{
    int i = 1;
    int status;
    int ans = 0;

    while (i < argc) {
        if (argv[i][0] == 'p') {
            while (i < argc && argv[i][0] == 'p') {
                pid_t pid = fork();

                if (pid == 0) {
                    execlp(argv[i] + 1, argv[i] + 1, NULL);
                    _exit(1);
                }

                i += 1;
            }

            while (wait(&status) > 0) {
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    ans += 1;
                }
            }

        } else if (argv[i][0] == 's') {
            pid_t pid = fork();

            if (pid == 0) {
                execlp(argv[i] + 1, argv[i] + 1, NULL);
                _exit(1);
            }

            i += 1;
            wait(&status);

            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                ans += 1;
            }
        }
    }

    printf("%d\n", ans);

    return 0;
}