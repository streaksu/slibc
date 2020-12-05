#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vfprintf(stdout, format, args);
    va_end(args);
    return ret;
}

int vprintf(const char *format, va_list args) {
    return vfprintf(stdout, format, args);
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
    fputs(buffer, stream);
    return ret;
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

int vsprintf(char *result, const char *format, va_list args) {
    return vsnprintf(result, SIZE_MAX, format, args);
}

/*
 * Everything below here has been stolen (allowed in on their license) and
 * lightly modified from lvlibc, function made by https://github.com/qookei.
 * Feel free to distribute it around, as it is a great self-contained snippet
 * of good stuff.
 */
#define FMT_PUT(dst, len, c) {\
                if(!len) goto end;\
                *(dst)++ = (c);\
                len--;\
                ret++;\
            }

static const char *digits_upper = "0123456789ABCDEF";
static const char *digits_lower = "0123456789abcdef";

static char *num_fmt(char *buf, size_t buf_len, uint64_t i, int base, int padding, char pad_with, int handle_signed, int upper, int len, char plus_char) {
    int neg = (int64_t)i < 0 && handle_signed;

    if (neg)
        i = (uint64_t)(-((int64_t)i));

    char *ptr = buf + buf_len - 1;
    *ptr = '\0';

    const char *digits = upper ? digits_upper : digits_lower;

    do {
        *--ptr = digits[i % base];
        if (padding)
            padding--;
        if (len > 0)
            len--;
        buf_len--;
    } while ((i /= base) != 0 && (len == -1 || len) && buf_len);

    while (padding && buf_len) {
        *--ptr = pad_with;
        padding--;
        buf_len--;
    }

    if (!buf_len)
        return ptr;

    if (neg)
        *--ptr = '-';

    if (!neg && plus_char)
        *--ptr = plus_char;

    return ptr;
}

#define NUM_BUF_LEN 48

int vsnprintf(char *buf, size_t len, const char *fmt, va_list arg) {
    int ret = 0;
    uint64_t i;
    char *s;
    char num_buf[NUM_BUF_LEN];

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
        char plus_char = 0;
        int alternative = 0;
        int precision = 0;

        if (*fmt == '0') {
            pad_with = '0';
            fmt++;
        } else if (*fmt == '#') {
            alternative = 1;
            fmt++;
        } else if (*fmt == ' ') {
            plus_char = ' ';
            fmt++;
        } else if (*fmt == '+') {
            plus_char = '+';
            fmt++;
        } else if (*fmt == '-') {
            // TODO: left align with padding
            fmt++;
        }

        if (*fmt == '*') {
            padding = va_arg(arg, int);
            fmt++;
        } else while (isdigit(*fmt)) {
            padding *= 10;
            padding += *fmt++ - '0';
        }

        // next character is the precision
        if (*fmt == '.') {
            fmt++;
            if (*fmt == '*') {
                precision = va_arg(arg, int);
                fmt++;
            } else while (isdigit(*fmt)) {
                precision *= 10;
                precision += *fmt++ - '0';
            }
        } else {
            precision = -1;
        }

        while (*fmt == 'l') {
            wide = 1;
            fmt++;
        }

        while (*fmt == 'h') {
            // XXX: handle narrower types in a special way?
            fmt++;
        }

        upper = *fmt == 'X' || *fmt == 'P';

        switch (*fmt) {
            case 'c': {
                i = va_arg(arg, int);
                FMT_PUT(buf, len, i);
                break;
            }

            case 'i':
            case 'd': {
                if (wide)
                    i = va_arg(arg, long int);
                else
                    i = va_arg(arg, int);

                char *c = num_fmt(num_buf, NUM_BUF_LEN, i, 10, padding, pad_with, 1, 0, -1, plus_char);
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
                    i = va_arg(arg, int) & 0xFFFFFFFFul;

                char *c = num_fmt(num_buf, NUM_BUF_LEN, i, 10, padding, pad_with, 0, 0, -1, plus_char);
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

                char *c = num_fmt(num_buf, NUM_BUF_LEN, i, 8, padding, pad_with, 0, 0, -1, plus_char);

                if (alternative)
                    FMT_PUT(buf, len, '0');

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

                char *c = num_fmt(num_buf, NUM_BUF_LEN, i, 16, padding, pad_with, 0, upper, wide ? 16 : 8, plus_char);

                if (alternative) {
                    FMT_PUT(buf, len, '0');
                    FMT_PUT(buf, len, upper ? 'X' : 'x');
                }

                while (*c) {
                    FMT_PUT(buf, len, *c);
                    c++;
                }
                break;
            }

            case 'P':
            case 'p': {
                i = (uintptr_t)(va_arg(arg, void *));

                char *c = num_fmt(num_buf, NUM_BUF_LEN, i, 16, padding, pad_with, 0, upper, sizeof(uintptr_t) / 4, plus_char);
                while (*c) {
                    FMT_PUT(buf, len, *c);
                    c++;
                }
                break;
            }

            case 's': {
                s = va_arg(arg, char *);
                while (*s && precision) {
                    FMT_PUT(buf, len, *s);
                    s++;
                    if (precision > 0)
                        precision--;
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
    if (len)
        *buf++ = '\0';

    return ret;
}
