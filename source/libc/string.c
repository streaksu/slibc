#include <string.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str) {
    int i;
    for (i = 0; str[i]; i++);
    return i;
}

int strcmp(const char *str1, const char *str2) {
    for (size_t i = 0; ; i++) {
        char c1 = str1[i];
        char c2 = str2[i];
        if (c1 != c2) {
            return c1 < c2 ? -1 : 1;
        }
        if (!c1) {
            return 0;
        }
    }
}

int strncmp(const char *str1, const char *str2, size_t num) {
    for (size_t i = 0; i < num; i++) {
        char c1 = str1[i];
        char c2 = str2[i];
        if (c1 != c2) {
            return c1 < c2 ? -1 : 1;
        }
        if (!c1) {
            return 0;
        }
    }

    return 0;
}

char *strcpy(char *destination, const char *source) {
    size_t i;
    for (i = 0; source[i]; i++) {
        destination[i] = source[i];
    }
    destination[i] = '\0';
    return destination;
}

void *memset(void *destination, int value, size_t count) {
    uint8_t *ptr = destination;

    for (size_t i = 0; i < count; i++) {
        ptr[i] = (uint8_t)value;
    }

    return destination;
}

void *memcpy(void *destination, const void *source, size_t count) {
    uint8_t       *dest = destination;
    const uint8_t *src  = source;

    for (size_t i = 0; i < count; i++) {
        dest[i] = src[i];
    }

    return dest;
}

int memcmp(const void *source1, const void *source2, size_t count) {
    const uint8_t *src1 = source1;
    const uint8_t *src2 = source2;

    for (size_t i = 0; i < count; i++) {
        if (src1[i] != src2[i]) {
            return src1[i] < src2[i] ? -1 : 1;
        }
    }

    return 0;
}

void *memmove(void *destination, const void *source, size_t count) {
    uint8_t       *dest = destination;
    const uint8_t *src  = source;

    if (source > destination) {
        for (size_t i = 0; i < count; i++) {
            dest[i] = src[i];
        }
    } else if (source < destination) {
        for (size_t i = count; i > 0; i--) {
            dest[i - 1] = src[i - 1];
        }
    }

    return dest;
}
