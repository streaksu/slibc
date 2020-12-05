#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

// TODO: Actually use locales for this.
// #define CURRENT_UTF8 (!!__pthread_self()->locale->cat[LC_CTYPE])
// #define MB_CUR_MAX (CURRENT_UTF8 ? 4 : 1)
#define MB_CUR_MAX 4

#ifdef __cplusplus
extern "C" {
#endif

void abort(void);
void exit(int status);
void _Exit(int status);

void *malloc(size_t size);
void *calloc(size_t count, size_t size);
void *realloc(void *pointer, size_t size);
void  free(void *pointer);

char *getenv(const char *name);
int   atoi(const char *str);
long  strtol(const char *str, char **endptr, int base);

void qsort(void *base, size_t count, size_t width, int (*compare)(const void *, const void *));

#ifdef __cplusplus
}
#endif

#endif
