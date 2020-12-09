#ifndef __STDIO_H__
#define __STDIO_H__

#include <stddef.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/fcntlval.h>

#ifdef __cplusplus
extern "C" {
#endif

#define getc fgetc
#define putc fputc
#define EOF  (-1)

#define FILE_BUFFER_SIZE 512

typedef struct {
    int    inner_fd;
    int    is_readable;
    int    is_writable;
    int    is_eof;
    int    is_error;
    size_t buffer_base;
    size_t bytes_in_buffer;
    size_t buffer_i;
    char   buffer[FILE_BUFFER_SIZE];
} FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

FILE  *fdopen(int fd, const char *mode);
FILE  *fopen(const char *filename, const char *mode);
int    fclose(FILE *stream);
int    fputc(int character, FILE *stream);
int    fgetc(FILE *stream);
int    fflush(FILE *stream);
int    fseek(FILE *stream, long offset, int where);
long   ftell(FILE *stream);
int    fputs(const char *str, FILE *stream);
char  *fgets(char *result, int count, FILE *stream);
size_t fwrite(const void *pointer, size_t size, size_t nitems, FILE *stream);
size_t fread(void *pointer, size_t size, size_t nitems, FILE *stream);
int    feof(FILE *stream);
int    ferror(FILE *stream);
void   clearerr(FILE *stream);
int    fileno(FILE *stream);
int    putchar(int character);
int    getchar(void);
int    puts(const char *str);
int    remove(const char *pathname);
int    sscanf(const char *str, const char *format, ...);

int printf(const char *format, ...);
int vprintf(const char *format, va_list args);
int fprintf(FILE *stream, const char *format, ...);
int vfprintf(FILE *stream, const char *format, va_list args);
int sprintf(char *result,   const char *format, ...);
int snprintf(char *result,  size_t count, const char *format, ...);
int vsprintf(char *result, const char *format, va_list args);
int vsnprintf(char *result, size_t count, const char *format, va_list args);

#ifdef __cplusplus
}
#endif

#endif
