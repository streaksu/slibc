#ifndef __SYS__TERMIOSVAL_H__
#define __SYS__TERMIOSVAL_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

#define NCCS    11
#define VEOF     0
#define VEOL     1
#define VERASE   2
#define VINTR    3
#define VKILL    4
#define VMIN     5
#define VQUIT    6
#define VSTART   7
#define VSTOP    8
#define VSUSP    9
#define VTIME   10

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t     c_cc[NCCS];
    speed_t  ibaud;
    speed_t  obaud;
};

#ifdef __cplusplus
}
#endif

#endif
