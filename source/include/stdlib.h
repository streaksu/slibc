#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <stddef.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#ifdef __cplusplus
extern "C" {
#endif

void *malloc(size_t size);
void *calloc(size_t count, size_t size);
void *realloc(void *pointer, size_t size);
void  free(void *pointer);
void  abort(void);
void  exit(int status);
void  _Exit(int status);

#ifdef __cplusplus
}
#endif

#endif
