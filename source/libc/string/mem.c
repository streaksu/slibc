#include <string.h>
#include <stddef.h>
#include <stdint.h>

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
