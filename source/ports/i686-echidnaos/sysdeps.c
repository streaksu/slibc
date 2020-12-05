#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/api.h>
#include <sys/stat.h>
#include <time.h>
#include <stropts.h>
#include <termios.h>

int fcntl(int fd, int command, ...) {
    // TODO: Implement once echidnaOS supports it.
    (void)fd;
    (void)command;
    return -1;
}

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

int stat(const char *path, struct stat *result) {
    vfs_metadata_t metadata;

    if (OS_vfs_get_metadata(path, &metadata, VFS_FILE_TYPE) == VFS_FAILURE) {
        if (OS_vfs_get_metadata(path, &metadata, VFS_DIRECTORY_TYPE) == VFS_FAILURE) {
             if (OS_vfs_get_metadata(path, &metadata, VFS_DEVICE_TYPE) == VFS_FAILURE) {
                return ENOENT;
            } else {
                result->st_mode = S_IFBLK;
            }
        } else {
            result->st_mode = S_IFDIR;
        }
    } else {
        result->st_mode = S_IFREG;
    }

    result->st_dev = 0;
    result->st_ino = 0;
    result->st_nlink = 0;
    result->st_uid = 0;
    result->st_gid = 0;
    result->st_rdev = 0;
    result->st_size = metadata.size;
    result->st_atime = 0;
    result->st_mtime = 0;
    result->st_ctime = 0;
    result->st_blksize = 32768;
    result->st_blocks = metadata.size / 32768 + 1;

    return 0;
}

int fstat(int fd, struct stat *result) {
    // TODO: Implement when echidnaOS supports it.
    (void)fd;
    (void)result;
    assert(!"This is a stub");
    return -1;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp) {
    // TODO: Implement this once echidnaOS supports it.
    (void)clock_id;
    (void)tp;
    return -1;
}

int sched_yield(void) {
    // TODO: Implement this once echidnaOS has it, if ever.
    return -1;
}

int access(const char *path, int amode) {
    (void)amode;

    int fd = open(path, O_RDWR);
    if (fd == -1) {
        return -1;
    }

    close(fd);
    return 0;
}

int pipe(int fds[2]) {
    // TODO: Implement when echidnaOS supports it.
    (void)fds;
    assert(!"This is a stub");
    return -1;
}

pid_t getpid(void) {
    pid_t pid;
    asm volatile (
        "int $0x80"
        : "=a"(pid)
        : "a"(0x15)
        : "edx", "memory"
    );
    return pid;
}

pid_t getppid(void) {
    // TODO: Implement when echidnaOS supports it.
    assert(!"This is a stub");
    return 0;
}

pid_t getpgrp(void) {
    // TODO: Implement when echidnaOS supports it.
    assert(!"This is a stub");
    return 0;
}

gid_t getgid(void) {
    // TODO: Implement when echidnaOS supports it.
    assert(!"This is a stub");
    return 0;
}

int setgid(gid_t gid) {
    // TODO: Implement when echidnaOS supports it.
    (void)gid;
    assert(!"This is a stub");
    return -1;
}

gid_t getegid(void) {
    // TODO: Implement when echidnaOS supports it.
    assert(!"This is a stub");
    return 0;
}

uid_t getuid(void) {
    // TODO: Implement when echidnaOS supports it.
    assert(!"This is a stub");
    return 0;
}

int setuid(uid_t uid) {
    // TODO: Implement when echidnaOS supports it.
    (void)uid;
    assert(!"This is a stub");
    return -1;
}

uid_t geteuid(void) {
    // TODO: Implement when echidnaOS supports it.
    assert(!"This is a stub");
    return 0;
}

int execve(const char *path, char *const argv[], char *const envp[]) {
    int discard1, discard2;

    asm volatile (
        "int $0x80"
        : "=a"(discard1), "=d"(discard2)
        : "a"(0x03), "c"(path), "d"(argv), "D"(envp)
        : "memory"
    );

    return EIO;
}

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp) {
    // TODO: Implement when echidnaOS supports it.
    (void)rqtp;
    (void)rmtp;
    assert(!"This is a stub");
    return -1;
}

int ttyname_r(int fd, char *name, size_t namesize) {
    // TODO: Implement when echidnaOS supports it.
    (void)fd;
    (void)name;
    (void)namesize;
    assert(!"This is a stub");
    return -1;
}

unsigned alarm(unsigned seconds) {
    // TODO: Implement when echidnaOS supports it.
    (void)seconds;
    assert(!"This is a stub");
    return 0;
}

int kill(pid_t pid, int signal) {
    // TODO: Implement when echidnaOS supports it.
    (void)pid;
    (void)signal;
    assert(!"This is a stub");
    return -1;
}

int ioctl(int fd, int request, ...) {
    // TODO: Implement when echidnaOS supports it.
    (void)fd;
    (void)request;
    assert(!"This is a stub");
    return -1;
}

int tcgetattr(int fd, struct termios *result) {
    // TODO: Implement when echidnaOS supports it.
    (void)fd;
    (void)result;
    assert(!"This is a stub");
    return -1;
}

int tcsetattr(int fd, int action, const struct termios *result) {
    // TODO: Implement when echidnaOS supports it.
    (void)fd;
    (void)action;
    (void)result;
    assert(!"This is a stub");
    return -1;
}

int tcflow(int fd, int action) {
    // TODO: Implement when echidnaOS supports it.
    (void)fd;
    (void)action;
    assert(!"This is a stub");
    return -1;
}
