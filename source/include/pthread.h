#ifndef __PTHREAD_H__
#define __PTHREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int placeholder;
} pthread_mutexattr_t;

int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

#define PTHREAD_MUTEX_INITIALIZER {0, {0}}
#define PTHREAD_MUTEX_RECURSIVE   0

typedef struct {
    int                 locked;
    pthread_mutexattr_t attr;
} pthread_mutex_t;

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

#define PTHREAD_ONCE_INIT {0}

typedef struct {
    int placeholder;
} pthread_once_t;

int pthread_once(pthread_once_t *once, void (*init_routine)(void));

typedef struct {
    int placeholder;
} pthread_condattr_t;

typedef struct {
    int placeholder;
} pthread_cond_t;

struct timespec {
    int placeholder;
};

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);

typedef struct {
    int placeholder;
} pthread_attr_t;

typedef struct {
    int placeholder;
} pthread_t;

int       pthread_cancel(pthread_t thread);
int       pthread_detach(pthread_t thread);
int       pthread_equal(pthread_t thread1, pthread_t thread2);
int       pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(entrypoint)(void *), void *arg);
pthread_t pthread_self(void);
int       pthread_join(pthread_t thread, void **value_ptr);

typedef struct {
    int placeholder;
} pthread_key_t;

int   pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int   pthread_key_delete(pthread_key_t key);
int   pthread_setspecific(pthread_key_t key, const void *specific);
void *pthread_getspecific(pthread_key_t key);

int sched_yield(void);

#ifdef __cplusplus
}
#endif

#endif
