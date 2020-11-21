#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern int main(int argc, char *argv[], char *envp[]);

void __slibc_init(int argc, char *argv[], char *envp[]) {
    // Initialize std* buffers.
    stdin  = fdopen(0, "r");
    stdout = fdopen(1, "w");
    stderr = fdopen(2, "w");

    // Setup environ.
    environ = envp;

    // Call main and exit with its result.
    int exit_code = main(argc, argv, envp);
    exit(exit_code);
}
