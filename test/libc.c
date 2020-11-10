#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CWD_LEN 300

int main(int argc, char *argv[]) {
    char cwd[CWD_LEN];
    printf("Running from: %s\n", getcwd(cwd, CWD_LEN));

    void *lel = malloc(30);
    for (int i = 0; i < argc; i++) {
        puts(argv[i]);
    }
    free(lel);

    printf("Base     = %p\n", sbrk(0));
    printf("New      = %p\n", sbrk(35));
    printf("New new  = %p\n", sbrk(-35));
    printf("New      = %p\n", sbrk(0));

    return 0;
}
