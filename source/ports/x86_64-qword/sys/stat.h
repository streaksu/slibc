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
    mode_t          st_mode;
    nlink_t         st_nlink;
    uid_t           st_uid;
    gid_t           st_gid;
    dev_t           st_rdev;
    off_t           st_size;
    struct timespec st_atim;
    struct timespec st_mtim;
    struct timespec st_ctim;
    blksize_t       st_blksize;
    blkcnt_t        st_blocks;
};

int stat(const char *path, struct stat *result);

#ifdef __cplusplus
}
#endif

#endif
