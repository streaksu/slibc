#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

// TODO: Using a fixed amount of parameters could become a bug in the near
// future.

static void get_args(char *argv[], char **envp[], const char *arg0, va_list args) {
    size_t i  = 0;
    argv[i++] = (char *)arg0;

    if (arg0 != NULL) {
        for (;;) {
            char *arg = va_arg(args, char *);
            if (arg == NULL) {
                break;
            }
            argv[i++] = arg;
        }
    }

    argv[i++] = NULL;
    if (envp != NULL) {
        *envp = va_arg(args, char **);
    }
}

int execl(const char *path, const char *arg0, ...) {
    va_list args;
    va_start(args, arg0);

    char *argv[40];
    get_args(argv, NULL, arg0, args);

    va_end(args);
    return execve(path, argv, environ);
}

int execv(const char *path, char *const argv[]) {
    return execve(path, argv, environ);
}

int execle(const char *path, const char *arg0, ...) {
    va_list args;
    va_start(args, arg0);

    char *argv[40];
    char **envp;
    get_args(argv, &envp, arg0, args);

    va_end(args);
    return execve(path, argv, envp);
}

int execlp(const char *file, const char *arg0, ...) {
    va_list args;
    va_start(args, arg0);

    char *argv[40];
    get_args(argv, NULL, arg0, args);

    va_end(args);
    return execvpe(file, argv, environ);
}

int execvp(const char *file, char *const argv[]) {
    return execvpe(file, argv, environ);
}

int execvpe(const char *file, char *const argv[], char *const envp[]) {
    if (strchr(file, '/')) {
        return execve(file, argv, envp);
    }

    char *search_path = getenv("PATH");
    if (search_path == NULL) {
        search_path = "/bin:/usr/bin";
    }

    for (;;) {
        char  path[100]   = {0};
        char *curr_search = strchr(search_path, '/');
        char *curr_end    = strchr(search_path, ':');
        size_t curr_len   = curr_end - curr_search;
        strncpy(path, curr_search, curr_len);
        strcpy(path + curr_len, "/");
        strcpy(path + curr_len + 1, file);
        execve(path, argv, envp);
        if (errno == ENOEXEC) {
            char *shell_argv = {path, NULL};
            return execve(shell_argv[0], shell_argv, envp);
        } else if (curr_end == NULL) {
            break;
        } else {
            search_path = curr_end + 1;
        }
    }

    return -1;
}
