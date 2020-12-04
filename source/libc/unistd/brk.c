#include <unistd.h>
#include <stdint.h>

char **environ;

int brk(void *address) {
    intptr_t current_brk = (intptr_t)sbrk(0);
    intptr_t brk_diff    = (intptr_t)address - current_brk;
    if (sbrk(brk_diff) == (void*)-1) {
        return -1;
    }
    return 0;
}
