#ifndef __TERMIOS_H__
#define __TERMIOS_H__

#include <sys/termiosval.h>

#ifdef __cplusplus
extern "C" {
#endif

int tcgetattr(int fd, struct termios *result);
int tcsetattr(int fd, int action, const struct termios *result);
int tcflow(int fd, int action);

#ifdef __cplusplus
}
#endif

#endif
