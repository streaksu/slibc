#include <string.h>
#include <stddef.h>

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
