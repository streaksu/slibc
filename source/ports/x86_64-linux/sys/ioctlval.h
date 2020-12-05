#ifndef __SYS__IOCTLVAL_H__
#define __SYS__IOCTLVAL_H__

#define TCGETS     0x5401
#define TCSETS     0x5402
#define TCXONC     0x540A
#define TIOCGWINSZ 0x5413
#define TIOCSWINSZ 0x5414

struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};

#endif
