#ifndef __SYS__TIMES_H__
#define __SYS__TIMES_H__

#include <sys/types.h>
#include <sys/timesval.h>

#ifdef __cplusplus
extern "C" {
#endif

clock_t times(struct tms *t); // Defined by port.

#ifdef __cplusplus
}
#endif

#endif
