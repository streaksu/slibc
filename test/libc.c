#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char papa[300];
    printf("Running from: %s\n", getcwd(papa, 300));

    void *lel = malloc(30);
    for (int i = 0; i < argc; i++) {
        puts(argv[i]);
    }
    free(lel);

    return 0;
}
