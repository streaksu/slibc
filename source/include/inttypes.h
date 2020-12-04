#ifndef __INTTYPES_H__
#define __INTTYPES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Pretty sure these macros are self explanatory.
#define PRIx64 "PLACEHOLDER PLEASE IMPLEMENT"
#define PRId64 "PLACEHOLDER PLEASE IMPLEMENT"
#define PRIu64 "PLACEHOLDER PLEASE IMPLEMENT"

typedef struct {
    intmax_t quot;
    intmax_t rem;
} imaxdiv_t;

imaxdiv_t imaxdiv(intmax_t numerator, intmax_t denominator);
intmax_t  imaxabs(intmax_t number);

#ifdef __cplusplus
}
#endif

#endif
