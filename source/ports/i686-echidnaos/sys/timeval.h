#ifndef __SYS__TIMEVAL_H__
#define __SYS__TIMEVAL_H__

#include <sys/types.h>

#define CLOCK_REALTIME  0
#define CLOCK_MONOTONIC 0

#ifdef __cplusplus
extern "C" {
#endif

struct timespec {
    time_t tv_sec;
    long   tv_nsec;
};

#ifdef __cplusplus
}
#endif

#endif
