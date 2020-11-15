#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define stringify(a) str(a)
#define str(a)       #a

#ifdef NDEBUG
    #define assert(expr) ((void)0)
#else
    #define assert(expr) do { \
        if (!(expr)) { \
            puts("Assertion failed at line " stringify(__LINE__) " of file " stringify(__FILE__)); \
            exit(1); \
        } \
    } while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif
