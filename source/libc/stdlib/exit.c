#include <stdlib.h>
#include <unistd.h>

void abort(void) {
    _exit(EXIT_FAILURE); // DEATH DEATH DEATH DEATH.
}

void exit(int status) {
    // TODO: Call the finalizers, all that stuff.
    _exit(status);
}

void _Exit(int status) {
    _exit(status);
}
