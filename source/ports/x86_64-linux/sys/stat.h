#ifndef __SYS__STAT_H__
#define __SYS__STAT_H__

#include <sys/types.h>

struct stat {
    dev_t     st_dev;
    ino_t     st_ino;
    nlink_t   st_nlink;
    mode_t    st_mode;
    uid_t     st_uid;
    gid_t     st_gid;
    uint32_t  __pad0;
    dev_t     st_rdev;
    off_t     st_size;
    blksize_t st_blksize;
    blkcnt_t  st_blocks;
    // TODO: This should be timespec and not time_t.
    time_t    st_atime;
    time_t    st_mtime;
    time_t    st_ctime;
    // TODO: Once they use timespec this is for padding it.
    // Since the linux kernel uses it the padding has to be perfect.
    // long __unused[3];
};

int stat(const char *path, struct stat *result);

#endif
