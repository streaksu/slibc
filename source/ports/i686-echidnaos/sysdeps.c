#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/api.h>
#include <sys/stat.h>

int open(const char *path, int oflag, ...) {
    va_list args;
    va_start(args, oflag);

    mode_t mode = 0;
    if (oflag & O_CREAT) {
        mode = va_arg(args, mode_t);
    }

    return OS_open(path, oflag, mode);
}

int close(int fd) {
    return OS_close(fd);
}

ssize_t read(int fd, void *buf, size_t count) {
    return (ssize_t)OS_read(fd, buf, count);
}

ssize_t write(int fd, const void *buf, size_t count) {
    return (ssize_t)OS_write(fd, buf, count);
}

char *getcwd(char *buffer, size_t size) {
    (void)size;
    OS_pwd(buffer);
    return buffer;
}

int rmdir(const char *path) {
    return OS_vfs_remove(path);
}

int unlink(const char *path) {
    return OS_vfs_remove(path);
}

int fork(void) {
    pid_t ret;
    asm volatile ("int $0x80"
        : "=a"(ret)
        : "a"(0x05)
        : "edx", "memory");
    return ret;
}

void _exit(int error_code) {
    int discard;
    asm volatile ("int $0x80"
        : "=a"(discard)
        : "a"(0x00), "c"(error_code)
        : "edx", "memory"
    );
}

off_t lseek(int fd, off_t offset, int whence) {
    int return_val;                           
    asm volatile (  "movl $0x2e, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (return_val)        
                     : "c" (fd), 
                       "d" (offset),
                       "D" (whence)
                     :  );        
    return return_val;                               
}

void *sbrk(intptr_t size) {
    size_t ptr = OS_get_heap_base() + OS_get_heap_size();
    if (OS_resize_heap(OS_get_heap_size() + size) == -1) {
        errno = ENOMEM;
        return (void *)-1;
    }
    return (void *)ptr;
}

int stat(fsfd_target fsfdt, int fd, const char *path, int flags, struct stat *st) {
    vfs_metadata_t metadata;

    if (OS_vfs_get_metadata(path, &metadata, VFS_FILE_TYPE) == VFS_FAILURE) {
        if (OS_vfs_get_metadata(path, &metadata, VFS_DIRECTORY_TYPE) == VFS_FAILURE) {
             if (OS_vfs_get_metadata(path, &metadata, VFS_DEVICE_TYPE) == VFS_FAILURE) {
                return ENOENT;
            } else {
                st->st_mode = S_IFBLK;
            }
        } else {
            st->st_mode = S_IFDIR;
        }
    } else {
        st->st_mode = S_IFREG;
    }

    st->st_dev = 0;
    st->st_ino = 0;
    st->st_nlink = 0;
    st->st_uid = 0;
    st->st_gid = 0;
    st->st_rdev = 0;
    st->st_size = metadata.size;
    st->st_atime = 0;
    st->st_mtime = 0;
    st->st_ctime = 0;
    st->st_blksize = 32768;
    st->st_blocks = metadata.size / 32768 + 1;

    return 0;
}
