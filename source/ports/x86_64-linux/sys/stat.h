#ifndef __SYS__STAT_H__
#define __SYS__STAT_H__

#include <time.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct stat {
    dev_t           st_dev;
    ino_t           st_ino;
    nlink_t         st_nlink;
    mode_t          st_mode;
    uid_t           st_uid;
    gid_t           st_gid;
    uint32_t        __pad0;
    dev_t           st_rdev;
    off_t           st_size;
    blksize_t       st_blksize;
    blkcnt_t        st_blocks;
    struct timespec st_atime;
    struct timespec st_mtime;
    struct timespec st_ctime;
    long            __unused[3];
};

int stat(const char *path, struct stat *result);

#ifdef __cplusplus
}
#endif

#endif
