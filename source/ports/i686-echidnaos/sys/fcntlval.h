#ifndef __SYS__FCNTLVAL_H__
#define __SYS__FCNTLVAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define O_RDONLY (1 << 0)
#define O_WRONLY (1 << 1)
#define O_RDWR   (1 << 2)
#define O_APPEND (1 << 3)
#define O_CREAT  (1 << 4)
#define O_TRUNC  (1 << 5)
#define O_EXCL   (1 << 6)

#define SEEK_SET 0
#define SEEK_END 1
#define SEEK_CUR 2

#define F_SETFD 69 // Placeholder.

#define F_OK 1
#define R_OK 2
#define W_OK 4
#define X_OK 8

struct flock {
    short l_type;
    short l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
};

#ifdef __cplusplus
}
#endif

#endif
