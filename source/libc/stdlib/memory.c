#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define BLOCK_SIZE        128
#define INITIAL_POOL_SIZE 128

struct malloc_metadata {
    size_t blocks;
    size_t size;
    char   data[];
};

static void           *bitmap            = NULL;
static size_t          bitmap_last_index = 0;
static uintptr_t       heap_base         = 0; 
static size_t          pool_size         = 0;
static pthread_mutex_t alloc_mutex       = PTHREAD_MUTEX_INITIALIZER;

#define SIZE_T_BITS       (sizeof(size_t) * 8)
#define DIV_ROUNDUP(a, b) (((a) + ((b) - 1)) / (b))

static int bitmap_test(void *bitmap, size_t num) {
    size_t index  = num / SIZE_T_BITS;
    size_t offset = num % SIZE_T_BITS;
    return ((size_t *)bitmap)[index] & ((size_t)1 << offset);
}

static void bitmap_set(void *bitmap, size_t num) {
    size_t index  = num / SIZE_T_BITS;
    size_t offset = num % SIZE_T_BITS;
    ((size_t *)bitmap)[index] |= (size_t)1 << offset;
}

static void bitmap_unset(void *bitmap, size_t num) {
    size_t index  = num / SIZE_T_BITS;
    size_t offset = num % SIZE_T_BITS;
    ((size_t *)bitmap)[index] &= ~((size_t)1 << offset);
}

static void *allocate_blocks_inner(size_t count, size_t limit) {
    size_t p = 0;

    while (bitmap_last_index < limit) {
        if (!bitmap_test(bitmap, bitmap_last_index++)) {
            if (++p == count) {
                size_t block = bitmap_last_index - count;
                for (size_t i = block; i < bitmap_last_index; i++) {
                    bitmap_set(bitmap, i);
                }
                return (void *)(block * BLOCK_SIZE + heap_base);
            }
        } else {
            p = 0;
        }
    }

    return NULL;
}

static void *allocate_blocks(size_t count) {
    size_t last_used   = bitmap_last_index;
    size_t bitmap_size = pool_size / 8;
    void *ret = allocate_blocks_inner(count, bitmap_size);
    if (ret == NULL) {
        bitmap_last_index = 0;
        ret = allocate_blocks_inner(count, last_used);
    }

    return ret;
}

static void *block_alloc(size_t block_count) {
    if (heap_base == 0) {
        heap_base = (uintptr_t)sbrk(0);
    }

    void *ret;
    while ((ret = allocate_blocks(block_count)) == NULL) {
        size_t old_bitmap_size = pool_size / 8;

        if (pool_size == 0) {
            pool_size = INITIAL_POOL_SIZE;
        } else {
            pool_size *= 2;
        }

        size_t new_bitmap_size = pool_size / 8;
        size_t new_heap_size   = pool_size * BLOCK_SIZE + new_bitmap_size;
        void  *new_bitmap      = (void *)((heap_base + new_heap_size) - new_bitmap_size);

        if (brk((void *)(heap_base + new_heap_size)) != 0) {
            return NULL;
        }

        memset(new_bitmap, 0, new_bitmap_size);
        memcpy(new_bitmap, bitmap, old_bitmap_size);
        bitmap = new_bitmap;
    }

    return ret;
}

static void block_free(void *ptr, size_t count) {
    size_t block = ((size_t)ptr - heap_base) / BLOCK_SIZE;
    for (size_t i = 0; i < count; i++) {
        bitmap_unset(bitmap, block + i);
    }
}

void *malloc(size_t size) {
    pthread_mutex_lock(&alloc_mutex);
    size_t block_count = DIV_ROUNDUP(sizeof(struct malloc_metadata) + size, BLOCK_SIZE);
    struct malloc_metadata *ptr = block_alloc(block_count);
    if (ptr == NULL) {
        pthread_mutex_unlock(&alloc_mutex);
        return NULL;
    }

    ptr->blocks = block_count;
    ptr->size   = size;

    pthread_mutex_unlock(&alloc_mutex);
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

    struct malloc_metadata *metadata = (void *)((char *)pointer - sizeof(struct malloc_metadata));
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

    struct malloc_metadata *metadata = (void *)((char *)pointer - sizeof(struct malloc_metadata));
    block_free(metadata, metadata->blocks);
}
