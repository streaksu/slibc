#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CWD_LEN 300

int main(int argc, char *argv[]) {
    char cwd[CWD_LEN];
    printf("Running from: %s\n", getcwd(cwd, CWD_LEN));
    for (int i = 0; i < argc; i++) {
        puts(argv[i]);
    }

    void *ptr1 = malloc(30);
    printf("%p\n", ptr1);
    void *ptr2 = malloc(0x1000000);
    printf("%p\n", ptr2);
    free(ptr1);
    ptr1 = malloc(30);
    printf("%p\n", ptr1);
    ptr2 = realloc(ptr2, 22);
    printf("%p\n", ptr2);

    return 0;
}
