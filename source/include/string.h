#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char *str);
int    strcmp(const char *str1, const char *str2);
int    strncmp(const char *str1, const char *str2, size_t num);
char  *strcpy(char *destination, const char *source);
char  *strncpy(char *destination, const char *source, size_t count);
char  *strcat(char *destination, const char *source);
char  *strchr(const char *string, int c);
char  *strrchr(const char *string, int c);
char  *strstr(const char *string, const char *searched);
char  *strpbrk(const char *string, const char *searched);
char  *strsignal(int signal);

void  *memset(void *destination, int value, size_t count);
void  *memcpy(void *destination, const void *source, size_t count);
int    memcmp(const void *source1, const void *source2, size_t count);
void  *memmove(void *destination, const void *source, size_t count);

#ifdef __cplusplus
}
#endif

#endif
