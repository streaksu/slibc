#ifndef __SYS__TYPES_H__
#define __SYS__TYPES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t dev_t;
typedef uint64_t ino_t;
typedef uint32_t mode_t;
typedef uint64_t nlink_t;
typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef int64_t  time_t;
typedef int64_t  suseconds_t;
typedef int64_t  blksize_t;
typedef int64_t  blkcnt_t;
typedef int32_t  pid_t;
typedef int64_t  off_t;
typedef int64_t  ssize_t;
typedef int32_t  clockid_t;

#ifdef __cplusplus
}
#endif

#endif
