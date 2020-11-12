#include <stdlib.h>
#include <stdio.h>

void __slibc_init(void) {
    // Initialize std* buffers.
    stdin  = fdopen(0, "r");
    stdout = fdopen(1, "w");
    stderr = fdopen(2, "w");
}
