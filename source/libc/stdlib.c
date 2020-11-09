#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

static pthread_mutex_t allocMutex = PTHREAD_MUTEX_INITIALIZER;

void *malloc(size_t size) {
    pthread_mutex_lock(&allocMutex);
    (void)size;
    pthread_mutex_unlock(&allocMutex);
    return NULL;
}

void *calloc(size_t count, size_t size) {
    void *ret = malloc(count * size);
    if (ret == NULL) {
        return NULL;
    }

    memset(ret, 0, count * size);
    return ret;
}

void *realloc(void *pointer, size_t size) {
    if (pointer == NULL) {
        return malloc(size);
    }
    // TODO: actual realloc.
    return NULL;
}

void free(void *pointer) {
    if (pointer == NULL) {
        return;
    }
}

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
