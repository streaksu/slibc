#ifndef __SYS__FCNTLVAL_H__
#define __SYS__FCNTLVAL_H__

#ifdef __cplusplus
extern "C" {
#endif

// reserve 3 bits for the access mode
#define O_ACCMODE 0x0007
#define O_EXEC 1
#define O_RDONLY 2
#define O_RDWR 3
#define O_SEARCH 4
#define O_WRONLY 5
// all remaining flags get their own bit
#define O_APPEND 0x0008
#define O_CREAT 0x0010
#define O_DIRECTORY 0x0020
#define O_EXCL 0x0040
#define O_NOCTTY 0x0080
#define O_NOFOLLOW 0x0100
#define O_TRUNC 0x0200
#define O_NONBLOCK 0x0400
#define O_DSYNC 0x0800
#define O_RSYNC 0x1000
#define O_SYNC 0x2000
#define O_CLOEXEC 0x4000

// constants for fcntl()'s command argument
#define F_DUPFD 1
#define F_DUPFD_CLOEXEC 2
#define F_GETFD 3
#define F_SETFD 4
#define F_GETFL 5
#define F_SETFL 6
#define F_GETLK 7
#define F_SETLK 8
#define F_SETLKW 9
#define F_GETOWN 10
#define F_SETOWN 11

// constants for struct flock's l_type member
#define F_RDLCK 1
#define F_UNLCK 2
#define F_WRLCK 3

// constants for fcntl()'s additional argument of F_GETFD and F_SETFD
#define FD_CLOEXEC 1

#define F_GETPATH 100

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 3

#ifdef __cplusplus
}
#endif

#endif
