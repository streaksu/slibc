#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <sys/signalval.h> // Kernel-specific signal info.
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*__sighandler)(int);

extern const int         sys_nsig;      // Defined by the port.
extern const char *const sys_siglist[]; // Defined by the port.

int kill(pid_t pid, int signal); // Defined by port.

#ifdef __cplusplus
}
#endif

#endif
