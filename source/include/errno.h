#ifndef __ERRNO_H__
#define __ERRNO_H__

#ifdef __cplusplus
extern "C" {
#endif

// The inner value of the possible values of errno are dictated by the port
// Following https://pubs.opengroup.org/onlinepubs/009695399/basedefs/errno.h.html
// for a basic implementation.
#include <sys/errnoval.h>

// TODO: Make this thread-local.
#define errno __errno
extern int __errno;

#ifdef __cplusplus
}
#endif

#endif
