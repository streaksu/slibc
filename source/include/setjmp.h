#ifndef SETJMP_H
#define SETJMP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __x86_64__
    typedef uint64_t jmp_buf[8]; /// Non volatile GPRs.
#elif __i386__
    typedef uint32_t jmp_buf[6]; /// Non volatile GPRs.
#else
    #error "Architecture not supported for setjmp"
#endif

void longjmp(jmp_buf state, int value);
int  setjmp(jmp_buf state);

#ifdef __cplusplus
}
#endif

#endif
