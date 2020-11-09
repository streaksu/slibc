#ifndef __SYS__FCNTLVAL_H__
#define __SYS__FCNTLVAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define O_RDONLY 0b000001
#define O_WRONLY 0b000010
#define O_RDWR   0b000100
#define O_APPEND 0b001000
#define O_CREAT  0b010000
#define O_TRUNC  0b100000

#define SEEK_SET 0
#define SEEK_END 1
#define SEEK_CUR 2

typedef int mode_t;

#ifdef __cplusplus
}
#endif

#endif
