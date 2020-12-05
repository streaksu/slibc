#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <sys/signalval.h> // Kernel-specific signal info.
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const char *const sys_siglist[]; // Defined by the port.

int kill(pid_t pid, int signal); // Defined by port.

#ifdef __cplusplus
}
#endif

#endif