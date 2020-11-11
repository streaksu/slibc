#ifndef __STDIO_H__
#define __STDIO_H__

#include <stddef.h>
#include <stdarg.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define getc fgetc
#define EOF  (-1)

typedef struct {
    int inner_fd;
} FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int    putchar(int character);
int    puts(const char *str);
int    printf(const char *format, ...);
FILE  *fopen(const char *restrict filename, const char *restrict mode);
int    fclose(FILE *stream);
int    fputc(int character, FILE *stream);
int    fputs(const char *str, FILE *stream);
int    fgetc(FILE *stream);
char  *fgets(char *restrict result, int count, FILE *restrict stream);
int    fprintf(FILE *stream, const char *format, ...);
int    vfprintf(FILE *stream, const char *format, va_list args);
int    fflush(FILE *stream);
int    fseek(FILE *stream, long offset, int whence);
off_t  ftell(FILE *stream);
size_t fwrite(const void *pointer, size_t size, size_t nitems, FILE *stream);
int    remove(const char *pathname);
int    sprintf(char *result,   const char *format, ...);
int    snprintf(char *result,  size_t count, const char *format, ...);
int    vsnprintf(char *result, size_t count, const char *format, va_list args);
int    getchar(void);

#ifdef __cplusplus
}
#endif

#endif
