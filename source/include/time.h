#ifndef __TIME_H__
#define __TIME_H__

#include <stddef.h>
#include <sys/types.h>

// The structures and clockid kernel-specific macros used in this file
// defined accordingly to
// https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/time.h.html
#include <sys/timeval.h>

#ifdef __cplusplus
extern "C" {
#endif

int    clock_gettime(clockid_t clock_id, struct timespec *tp); // Implemented by the port.
int    nanosleep(const struct timespec *rqtp, struct timespec *rmtp); // Ditto.
time_t time(time_t *store);

#ifdef __cplusplus
}
#endif

#endif
