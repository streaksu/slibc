#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/api.h>

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
    if (OS_resize_heap(OS_get_heap_size() + size)) == -1) {
        errno = ENOMEM;
        return (void *)-1;
    }
    return (void *)ptr;
}
