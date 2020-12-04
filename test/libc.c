#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <setjmp.h>

#define CWD_LEN 300

static jmp_buf buffer;

int main(int argc, char *argv[]) {
    char cwd[CWD_LEN];
    int ret = setjmp(buffer);
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
    free(ptr1);
    free(ptr2);

    printf("The value of SHELL is: %s\n", getenv("SHELL"));
    printf("The time is: %lu \n", time(NULL));

    if (ret == 0) longjmp(buffer, 0);

    pid_t f = fork();
    if (f == 0) {
        char *argv[] = {"ls", NULL};
        execvpe(argv[0], argv, environ);
    } else {
        printf("My proud child panicking is %i", f);
    }

    return 0;
}
