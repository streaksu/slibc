#ifndef __SYS__FCNTLVAL_H__
#define __SYS__FCNTLVAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define O_ACCMODE       00000003
#define O_RDONLY        00000000
#define O_WRONLY        00000001
#define O_RDWR          00000002
#define O_CREAT         00000100
#define O_EXCL          00000200
#define O_NOCTTY        00000400 
#define O_TRUNC         00001000
#define O_APPEND        00002000
#define O_NONBLOCK      00004000
#define O_DSYNC         00010000        /* used to be O_SYNC, see below */
#define FASYNC          00020000        /* fcntl, for BSD compatibility */
#define O_DIRECT        00040000        /* direct disk access hint */
#define O_LARGEFILE     00100000
#define O_DIRECTORY     00200000        /* must be a directory */
#define O_NOFOLLOW      00400000        /* don't follow links */
#define O_NOATIME       01000000
#define O_CLOEXEC       02000000        /* set close_on_exec */
/*
 * Before Linux 2.6.33 only O_DSYNC semantics were implemented, but using
 * the O_SYNC flag.  We continue to use the existing numerical value
 * for O_DSYNC semantics now, but using the correct symbolic name for it.
 * This new value is used to request true Posix O_SYNC semantics.  It is
 * defined in this strange way to make sure applications compiled against
 * new headers get at least O_DSYNC semantics on older kernels.
 *
 * This has the nice side-effect that we can simply test for O_DSYNC
 * wherever we do not care if O_DSYNC or O_SYNC is used.
 *
 * Note: __O_SYNC must never be used directly.
 */
#ifndef O_SYNC
#define __O_SYNC        04000000
#define O_SYNC          (__O_SYNC|O_DSYNC)
#endif

#ifndef O_PATH
#define O_PATH          010000000
#endif

#ifndef __O_TMPFILE
#define __O_TMPFILE     020000000
#endif

/* a horrid kludge trying to make sure that this will fail on old kernels */
#define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#define O_TMPFILE_MASK (__O_TMPFILE | O_DIRECTORY | O_CREAT)      

#ifndef O_NDELAY
#define O_NDELAY        O_NONBLOCK
#endif

#define SEEK_SET 0
#define SEEK_END 2
#define SEEK_CUR 1

#define F_SETFD 2

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

#ifdef __cplusplus
}
#endif

#endif
