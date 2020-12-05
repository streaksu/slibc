#ifndef __SYS__STAT_H__
#define __SYS__STAT_H__

#include <sys/statval.h> // Kernel-specific stat values.
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int stat(const char *path, struct stat *result); // Port.
int fstat(int fd, struct stat *result);          // Ditto.
int fchmod(int fd, mode_t mode);                 // Ditto.

#ifdef __cplusplus
}
#endif

#endif
