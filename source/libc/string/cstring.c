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

char *strncpy(char *destination, const char *source, size_t count) {
    size_t i;

    for (i = 0; i < count && source[i]; i++) {
        destination[i] = source[i];
    }

    for (; i < count; i++) {
        destination[i] = 0;
    }

    return destination;
}

char *strcat(char *destination, const char *source) {
    char *ret = destination;

    while (*destination != '\0') {
        destination++;
    }

    while ((*destination++ = *source++));

    return ret;
}

char *strchr(const char *string, int c) {
    for (size_t i = 0; ; i++) {
        if (string[i] == c) {
            return (char *)&string[i];
        }
        if (string[i] == 0) {
            return NULL;
        }
    }
}

char *strrchr(const char *string, int c) {
    for (size_t i = 0, last_index = 0; ; i++) {
        if (string[i] == c) {
            last_index = i;
        }
        if (string[i] == 0) {
            return (char *)&string[last_index];
        }
    }
}

char *strstr(const char *string, const char *searched) {
    size_t searched_len = strlen(searched);

    for (size_t i = 0; string[i]; i++) {
        if (string[i] != searched[i]) {
            continue;
        }
        if (strncmp(&string[i], searched, searched_len) == 0) {
            return (char *)&string[i];
        }
    }

    return NULL;
}
