#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void swap(void *x, void *y, size_t l) {
    char *a = x;
    char *b = y;
    while(l--) {
        char c = *a;
        *a++ = *b;
        *b++ = c;
    }
}

static void inner_sort(char *array, size_t size, int (*cmp)(const void*, const void*), int begin, int end) {
    if (end > begin) {
        void *pivot = array + begin;
        int l = begin + size;
        int r = end;
        while(l < r) {
            if (cmp(array + l, pivot) <= 0) {
                l += size;
            } else if (cmp(array + r, pivot) > 0)  {
                r -= size;
            } else if ( l < r ) {
                swap(array + l, array + r, size);
            }
        }
        l -= size;
        swap(array+begin, array+l, size);
        inner_sort(array, size, cmp, begin, l);
        inner_sort(array, size, cmp, r, end);
    }
}

void qsort(void *base, size_t count, size_t size, int (*compare)(const void *, const void *)) {
    if (count > 0) {
        inner_sort(base, size, compare, 0, (count - 1) * size);
    }
}
