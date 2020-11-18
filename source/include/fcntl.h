#ifndef __FCNTL_H__
#define __FCNTL_H__

#include <stdarg.h>
#include <sys/types.h>

// The inner value of the possible values of oflags are dictated by the port
// Following https://pubs.opengroup.org/onlinepubs/009695399/basedefs/fcntl.h.html
// for a basic implementation.
#include <sys/fcntlval.h>

#ifdef __cplusplus
extern "C" {
#endif

int creat(const char *path, mode_t mode);
int fcntl(int fd, int command, ...);        // Defined by the port.
int open(const char *path, int oflag, ...); // Defined by the port.

#ifdef __cplusplus
}
#endif

#endif
