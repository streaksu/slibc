#ifndef __SYS__API_H__
#define __SYS__API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>

#define VFS_FILE_TYPE      0
#define VFS_DIRECTORY_TYPE 1
#define VFS_DEVICE_TYPE    2
#define VFS_SUCCESS        0
#define VFS_FAILURE       -2

typedef struct {
    char     filename[2048];
    int      filetype;
    uint64_t size;
} vfs_metadata_t;

typedef struct {
    char*  path;
    char*  ti_stdin;
    char*  ti_stdout;
    char*  ti_stderr;
    char*  pwd;
    char*  name;
    char*  server_name;
    int    argc;
    char** argv;
} task_info_t;

uint16_t OS_new_segment(int arg1, int arg2, int arg3);
uint32_t OS_get_heap_base(void);
uint32_t OS_get_heap_size(void);
int      OS_resize_heap(size_t size);
int      OS_vdev_register(uint8_t *in, uint8_t *out, int *flag_in, int *flag_out);
int      OS_vdev_in_ready(void *value);
int      OS_vdev_out_ready(void *value);
int      OS_vdev_await(void);
int      OS_read(int fd, void *buffer, size_t len);
int      OS_close(int fd);
int      OS_open(const char *path, int flags, mode_t mode);
int      OS_write(int fd, const void *buffer, size_t len);
int      OS_vfs_mkdir(const char *path, int perms);
int      OS_vfs_create(const char *path, int perms);
int      OS_vfs_remove(const char *path);
int      OS_vfs_list(const char *path, vfs_metadata_t *metadata, size_t entry);
int      OS_vfs_write(const char *path, size_t loc, const void *value);
int      OS_vfs_read(const char *path, size_t loc);
int      OS_vfs_cd(const char *path);
int      OS_vfs_get_metadata(const char *path, vfs_metadata_t *metadata, int type);
void     OS_pwd(const char *value);
int      OS_general_execute_block(task_info_t *taskinfo);
int      OS_general_execute(task_info_t *taskinfo);
void     OS_what_stdin(const char *path);
void     OS_what_stdout(const char *path);
void     OS_what_stderr(const char *path);
pid_t    OS_ipc_resolve_name(const char *server);
void     OS_ipc_send_packet(pid_t pid, void *payload, size_t len);
pid_t    OS_getpid(void);

#ifdef __cplusplus
}
#endif

#endif
