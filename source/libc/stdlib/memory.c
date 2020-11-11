#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define BLOCK_SIZE        128
#define INITIAL_POOL_SIZE 128

struct __malloc_metadata {
    size_t blocks;
    size_t size;
    char   data[];
};

static void           *__bitmap            = NULL;
static size_t          __bitmap_last_index = 0;
static uintptr_t       __heap_base         = 0; 
static size_t          __pool_size         = 0;
static pthread_mutex_t __alloc_mutex       = PTHREAD_MUTEX_INITIALIZER;

#define SIZE_T_BITS       (sizeof(size_t) * 8)
#define DIV_ROUNDUP(a, b) (((a) + ((b) - 1)) / (b))

static int __bitmap_test(void *bitmap, size_t num) {
    size_t index  = num / SIZE_T_BITS;
    size_t offset = num % SIZE_T_BITS;
    return ((size_t *)bitmap)[index] & ((size_t)1 << offset);
}

static void __bitmap_set(void *bitmap, size_t num) {
    size_t index  = num / SIZE_T_BITS;
    size_t offset = num % SIZE_T_BITS;
    ((size_t *)bitmap)[index] |= (size_t)1 << offset;
}

static void __bitmap_unset(void *bitmap, size_t num) {
    size_t index  = num / SIZE_T_BITS;
    size_t offset = num % SIZE_T_BITS;
    ((size_t *)bitmap)[index] &= ~((size_t)1 << offset);
}

static void *__allocate_blocks_inner(size_t count, size_t limit) {
    size_t p = 0;

    while (__bitmap_last_index < limit) {
        if (!__bitmap_test(__bitmap, __bitmap_last_index++)) {
            if (++p == count) {
                size_t block = __bitmap_last_index - count;
                for (size_t i = block; i < __bitmap_last_index; i++) {
                    __bitmap_set(__bitmap, i);
                }
                return (void *)(block * BLOCK_SIZE + __heap_base);
            }
        } else {
            p = 0;
        }
    }

    return NULL;
}

static void *__allocate_blocks(size_t count) {
    size_t last_used   = __bitmap_last_index;
    size_t bitmap_size = __pool_size / 8;
    void *ret = __allocate_blocks_inner(count, bitmap_size);
    if (ret == NULL) {
        __bitmap_last_index = 0;
        ret = __allocate_blocks_inner(count, last_used);
    }

    return ret;
}

static void *__block_alloc(size_t block_count) {
    if (__heap_base == 0) {
        __heap_base = (uintptr_t)sbrk(0);
    }

    void *ret;
    while ((ret = __allocate_blocks(block_count)) == NULL) {
        size_t old_bitmap_size = __pool_size / 8;

        if (__pool_size == 0) {
            __pool_size = INITIAL_POOL_SIZE;
        } else {
            __pool_size *= 2;
        }

        size_t new_bitmap_size = __pool_size / 8;
        size_t new_heap_size   = __pool_size * BLOCK_SIZE + new_bitmap_size;
        void  *new_bitmap      = (void *)((__heap_base + new_heap_size) - new_bitmap_size);

        if (brk((void *)(__heap_base + new_heap_size)) != 0) {
            return NULL;
        }

        memset(new_bitmap, 0, new_bitmap_size);
        memcpy(new_bitmap, __bitmap, old_bitmap_size);
        __bitmap = new_bitmap;
    }

    return ret;
}

static void __block_free(void *ptr, size_t count) {
    size_t block = ((size_t)ptr - __heap_base) / BLOCK_SIZE;
    for (size_t i = 0; i < count; i++) {
        __bitmap_unset(__bitmap, block + i);
    }
}

void *malloc(size_t size) {
    pthread_mutex_lock(&__alloc_mutex);
    size_t block_count = DIV_ROUNDUP(sizeof(struct __malloc_metadata) + size, BLOCK_SIZE);
    struct __malloc_metadata *ptr = __block_alloc(block_count);
    if (ptr == NULL) {
        pthread_mutex_unlock(&__alloc_mutex);
        return NULL;
    }

    ptr->blocks = block_count;
    ptr->size   = size;

    pthread_mutex_unlock(&__alloc_mutex);
    return ptr->data;
}

void *calloc(size_t count, size_t size) {
    void *ret = malloc(count * size);
    if (ret == NULL) {
        return NULL;
    }

    memset(ret, 0, count * size);
    return ret;
}

void *realloc(void *pointer, size_t size) {
    if (pointer == NULL) {
        return malloc(size);
    }

    struct __malloc_metadata *metadata = (void *)((char *)pointer - sizeof(struct __malloc_metadata));
    if (DIV_ROUNDUP(metadata->size, BLOCK_SIZE) == DIV_ROUNDUP(size, BLOCK_SIZE)) {
        metadata->size = size;
        return pointer;
    }

    void *new_pointer = malloc(size);
    if (new_pointer == NULL) {
        return NULL;
    }

    if (metadata->size > size) {
        memcpy(new_pointer, pointer, size);
    } else {
        memcpy(new_pointer, pointer, metadata->size);
    }

    free(pointer);
    return new_pointer;
}

void free(void *pointer) {
    if (pointer == NULL) {
        return;
    }

    struct __malloc_metadata *metadata = (void *)((char *)pointer - sizeof(struct __malloc_metadata));
    __block_free(metadata, metadata->blocks);
}
