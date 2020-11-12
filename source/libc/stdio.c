#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

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

int fputs(const char *str, FILE *stream) {
    // XXX: Removed conditional branch by assuming EOF == -1.
    return write(stream->inner_fd, str, strlen(str));
}

char *fgets(char *restrict result, int count, FILE *restrict stream) {
    char *ret = result;
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
    return ret;
    /*int i = read(stream->inner_fd, result, count - 1);
    result[i] = '\0';
    return i;*/
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

#define FMT_PUT(dst, len, c) {\
                if(!(len)) goto end; \
                *(dst)++ = (c); \
                len--; \
            }
static const char *digits_upper = "0123456789ABCDEF";
static const char *digits_lower = "0123456789abcdef";

static char *num_fmt(uint64_t i, int base, int padding, char pad_with, int handle_signed, int upper, int len) {
    int neg = (signed)i < 0 && handle_signed;

    if (neg)
        i = (unsigned)(-((signed)i));

    static char buf[50];
    char *ptr = buf + 49;
    *ptr = '\0';

    const char *digits = upper ? digits_upper : digits_lower;

    do {
        *--ptr = digits[i % base];
        if (padding)
            padding--;
        if (len > 0)
            len--;
    } while ((i /= base) != 0 && (len == -1 || len));

    while (padding) {
        *--ptr = pad_with;
        padding--;
    }

    if (neg)
        *--ptr = '-';

    return ptr;
}

int vsnprintf(char *buf, size_t len, const char *fmt, va_list arg) {
    uint64_t i;
    char *s;

    while(*fmt && len) {
        if (*fmt != '%') {
            *buf++ = *fmt;
            fmt++;
            continue;
        }

        fmt++;
        int padding = 0;
        char pad_with = ' ';
        int wide = 0, upper = 0;

        if (*fmt == '0') {
            pad_with = '0';
            fmt++;
        }

        while (isdigit(*fmt)) {
            padding *= 10;			// noop on first iter
            padding += *fmt++ - '0';
        }

        while (*fmt == 'l') {
            wide = 1;
            fmt++;
        }

        upper = *fmt == 'X' || *fmt == 'P';

        switch (*fmt) {
            case 'c': {
                i = va_arg(arg, int);
                FMT_PUT(buf, len, i)
                break;
            }

            case 'd': {
                if (wide)
                    i = va_arg(arg, long int);
                else
                    i = va_arg(arg, int);

                char *c = num_fmt(i, 10, padding, pad_with, 1, 0, -1);
                while (*c) {
                    FMT_PUT(buf, len, *c);
                    c++;
                }
                break;
            }

            case 'u': {
                if (wide)
                    i = va_arg(arg, long int);
                else
                    i = va_arg(arg, int);

                char *c = num_fmt(i, 10, padding, pad_with, 0, 0, -1);
                while (*c) {
                    FMT_PUT(buf, len, *c);
                    c++;
                }
                break;
            }

            case 'o': {
                if (wide)
                    i = va_arg(arg, long int);
                else
                    i = va_arg(arg, int);

                char *c = num_fmt(i, 8, padding, pad_with, 0, 0, -1);
                while (*c) {
                    FMT_PUT(buf, len, *c);
                    c++;
                }
                break;
            }

            case 'X':
            case 'x': {
                if (wide)
                    i = va_arg(arg, long int);
                else
                    i = va_arg(arg, int);

                char *c = num_fmt(i, 16, padding, pad_with, 0, upper, wide ? 16 : 8);
                while (*c) {
                    FMT_PUT(buf, len, *c);
                    c++;
                }
                break;
            }

            case 'P':
            case 'p': {
                i = (uintptr_t)(va_arg(arg, void *));

                char *c = num_fmt(i, 16, padding, pad_with, 0, upper, 16);
                while (*c) {
                    FMT_PUT(buf, len, *c);
                    c++;
                }
                break;
            }

            case 's': {
                s = va_arg(arg, char *);
                while (*s) {
                    FMT_PUT(buf, len, *s);
                    s++;
                }
                break;
            }

            case '%': {
                FMT_PUT(buf, len, '%');
                break;
            }
        }

        fmt++;
    }

end:
    if (!len) {
        *buf++ = '.';	// requires extra reserved space
        *buf++ = '.';
        *buf++ = '.';
    }

    *buf++ = '\0';
    return len;
}

int getchar(void) {
    return fgetc(stdin);
}
