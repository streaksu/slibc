#include <pthread.h>
#include <stddef.h>
#include <assert.h>

#define LOCKED_VAL   1
#define UNLOCKED_VAL 0

int pthread_mutexattr_init(pthread_mutexattr_t *attr) {
    (void)attr;
    assert(!"Not implemented");
    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr) {
    (void)attr;
    assert(!"Not implemented");
    return 0;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type) {
    (void)attr;
    (void)type;
    assert(!"Not implemented");
    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type) {
    (void)attr;
    (void)type;
    assert(!"Not implemented");
    return 0;
}

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr) {
    mutex->locked = UNLOCKED_VAL;
    if (attr == NULL) {
        mutex->attr = (pthread_mutexattr_t){0};
    } else {
        mutex->attr = *attr;
    }
    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex) {
    (void)mutex;
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
    int expected = UNLOCKED_VAL;
    while (__atomic_compare_exchange_n(&mutex->locked, &expected, LOCKED_VAL, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED));
    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) {
    int expected = UNLOCKED_VAL;
    return __atomic_compare_exchange_n(&mutex->locked, &expected, LOCKED_VAL, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
    int expected = UNLOCKED_VAL;
    __atomic_store(&mutex->locked, &expected, __ATOMIC_RELAXED);
    return 0;
}

int pthread_once(pthread_once_t *once, void (*init_routine)(void)) {
    (void)once;
    (void)init_routine;
    assert(!"Not implemented");
    return 0;
}

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr) {
    (void)cond;
    (void)attr;
    assert(!"Not implemented");
    return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond) {
    (void)cond;
    assert(!"Not implemented");
    return 0;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
    (void)cond;
    (void)mutex;
    assert(!"Not implemented");
    return 0;
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime) {
    (void)cond;
    (void)mutex;
    (void)abstime;
    assert(!"Not implemented");
    return 0;
}

int pthread_cond_signal(pthread_cond_t *cond) {
    (void)cond;
    assert(!"Not implemented");
    return 0;
}

int pthread_cond_broadcast(pthread_cond_t *cond) {
    (void)cond;
    assert(!"Not implemented");
    return 0;
}

int pthread_cancel(pthread_t thread) {
    (void)thread;
    assert(!"Not implemented");
    return 0;
}

int pthread_detach(pthread_t thread) {
    (void)thread;
    assert(!"Not implemented");
    return 0;
}

int pthread_equal(pthread_t thread1, pthread_t thread2) {
    (void)thread1;
    (void)thread2;
    assert(!"Not implemented");
    return 0;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(entrypoint)(void *), void *arg) {
    (void)thread;
    (void)attr;
    (void)entrypoint;
    (void)arg;
    assert(!"Not implemented");
    return 0;
}

pthread_t pthread_self(void) {
    assert(!"Not implemented");
    pthread_t thread = {0};
    return thread;
}

int pthread_join(pthread_t thread, void **value_ptr) {
    (void)thread;
    (void)value_ptr;
    assert(!"Not implemented");
    return 0;
}

int pthread_key_create(pthread_key_t *key, void (*destructor)(void*)) {
    (void)key;
    (void)destructor;
    assert(!"Not implemented");
    return 0;
}

int pthread_key_delete(pthread_key_t key) {
    (void)key;
    assert(!"Not implemented");
    return 0;
}

int pthread_setspecific(pthread_key_t key, const void *specific) {
    (void)key;
    (void)specific;
    assert(!"Not implemented");
    return 0;
}

void *pthread_getspecific(pthread_key_t key) {
    (void)key;
    assert(!"Not implemented");
    return NULL;
}
