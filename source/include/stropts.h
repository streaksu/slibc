#ifndef __STROPTS_H__
#define __STROPTS_H__

#include <sys/ioctlval.h> // Kernel-specific ioctl values.

#ifdef __cplusplus
extern "C" {
#endif

int ioctl(int fd, int request, ...); // Port.

#ifdef __cplusplus
}
#endif

#endif
