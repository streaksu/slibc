#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *stdin  = &(FILE) { .inner_fd = 0 };
FILE *stdout = &(FILE) { .inner_fd = 1 };
FILE *stderr = &(FILE) { .inner_fd = 2 };

int putchar(int character) {
    if (write(stdout->inner_fd, &character, 1) == -1) {
        return EOF;
    } else {
        return character;
    }
}

int puts(const char *str) {
    const int fd = stdout->inner_fd;
    write(fd, str,  strlen(str));
    write(fd, "\n", 1);
    return 0;
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vfprintf(stdout, format, args);
    va_end(args);
    return ret;
}

FILE *fopen(const char *restrict filename, const char *restrict mode) {
    int flags;

    if (strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0) {
        flags = O_RDONLY;
    } else if (strcmp(mode, "w") == 0 || strcmp(mode, "wb") == 0) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (strcmp(mode, "a") == 0 || strcmp(mode, "ab") == 0) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    } else if (strcmp(mode, "r+") == 0 || strcmp(mode, "rb+") == 0 || strcmp(mode, "r+b") == 0) {
        flags = O_RDWR;
    } else if (strcmp(mode, "w+") == 0 || strcmp(mode, "wb+") == 0 || strcmp(mode, "w+b") == 0) {
        flags = O_RDWR | O_CREAT | O_TRUNC;
    } else if (strcmp(mode, "a+") == 0 || strcmp(mode, "ab+") == 0 || strcmp(mode, "a+b") == 0) {
        flags = O_RDWR | O_CREAT | O_APPEND;
    } else {
        errno = EINVAL;
        return NULL;
    }

    FILE *ret = malloc(sizeof(FILE));
    if (ret == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    ret->inner_fd = open(filename, flags);
    if (ret->inner_fd == -1) {
        return NULL;
    }

    return ret;
}

int fclose(FILE *stream) {
    int ret = close(stream->inner_fd);
    free(stream);
    return ret;
}

int fputc(int character, FILE *stream) {
    if (write(stream->inner_fd, &character, 1) == -1) {
        return EOF;
    } else {
        return character;
    }
}

int fputs(const char *str, FILE *stream) {
    // XXX: Removed conditional branch by assuming EOF == -1.
    return write(stream->inner_fd, str, strlen(str));
}

int fgetc(FILE *stream) {
    char c = EOF;
    if (read(stream->inner_fd, &c, 1) == -1)
        return EOF;
    return c;
}

char *fgets(char *restrict result, int count, FILE *restrict stream) {
    /*char *ret = result;
    int i;
    for (i = 0; i < count - 1; i++) {
        int c = fgetc(stream);
        if (c == EOF) {
            ret = NULL;
            break;
        }
        result[i] = c;
        if (c == '\n') {
            break;
        }
    }
    result[++i] = '\0';
    return ret;*/
    int i = read(stream->inner_fd, result, count - 1);
    result[i] = '\0';
    return i;
}

int fprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vfprintf(stream, format, args);
    va_end(args);
    return ret;
}

int vfprintf(FILE *stream, const char *format, va_list args) {
    char buffer[300];
    int ret = vsnprintf(buffer, 300, format, args);
    write(stream->inner_fd, buffer, strlen(buffer));
    return ret;
}

int fflush(FILE *stream) {
    (void)stream;
    return 0;
}

int fseek(FILE *stream, long offset, int whence) {
    if (lseek(stream->inner_fd, offset, whence) == -1) {
        return -1;
    }
    return 0;
}

off_t ftell(FILE *stream) {
    return lseek(stream->inner_fd, 0, SEEK_CUR);
}

size_t fwrite(const void *pointer, size_t size, size_t nitems, FILE *stream) {
    size_t         pointer_i = 0;
    const uint8_t *data      = pointer;
    for (size_t i = 0; i < nitems; i++) {
        for (size_t j = 0; j < size; j++) {
            if (fputc(data[pointer_i++], stream) == EOF) {
                return i;
            }
        }
    }
    return nitems;
}

int remove(const char *pathname) {
    int ret = unlink(pathname);
    return (ret && errno == EISDIR) ? rmdir(pathname) : ret;
}

int sprintf(char *result, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vsnprintf(result, SIZE_MAX, format, args);
    va_end(args);
    return ret;
}

int snprintf(char *result, size_t count, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vsnprintf(result, count, format, args);
    va_end(args);
    return ret;
}

int vsnprintf(char *result, size_t count, const char *format, va_list args) {
    size_t index = 0;
    for (size_t i = 0; i < count - 1 && format[i]; i++) {
        if (format[i] != '%') {
            result[index++] = format[i];
            continue;
        }

        i += 1;
        char *s;
        switch (format[i]) {
            case '\0':
                goto out;
            case '%':
                result[index++] = '%';
                break;
            case 's':
                s = va_arg(args, char*);
                strcpy(&result[index], s);
                index += strlen(s);
                break;
            default:
                result[index++] = '?';
        }
    }

out:
    result[index++] = '\0';
    return index;
}
