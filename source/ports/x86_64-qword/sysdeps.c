#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <sched.h>
#include <assert.h>
#include <stropts.h>
#include <termios.h>

int fcntl(int fd, int command, ...) {
    va_list args;
    va_start(args, command);
    int ret;
    int sys_errno;
    size_t arg = va_arg(args, size_t);

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(18), "D"(fd), "S"(command), "d"(arg)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return ret;
}

int open(const char *path, int oflag, ...) {
    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(1), "D"(path), "S"(oflag), "d"(0)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return ret;
}

int close(int fd) {
    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(2), "D"(fd)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return 0;
}

ssize_t read(int fd, void *buf, size_t count) {
    ssize_t ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(3), "D"(fd), "S"(buf), "d"(count)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return ret;
}

ssize_t write(int fd, const void *buf, size_t count) {
    ssize_t ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(3), "D"(fd), "S"(buf), "d"(count)
        : "rcx", "r11");

    if (ret == -1) {
        errno = sys_errno;
    }

    return ret;
}

char *getcwd(char *buf, size_t size) {
    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(20), "D"(buf), "S"(size)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return 0;
}

int rmdir(const char *path) {
    // TODO: Qword does not really support this.
    (void)path;
    return -1;
}

int unlink(const char *path) {
    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(34), "D"(path)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return 0;
}

int fork(void) {
    pid_t ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(10)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return ret;
}

void _exit(int error_code) {
    asm volatile ("syscall" :: "a"(12), "D"(error_code) : "rcx", "r11", "rdx");
}

off_t lseek(int fd, off_t offset, int whence) {
    off_t ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(8), "D"(fd), "S"(offset), "d"(whence)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return ret;
}

#define PAGE_SIZE 0x1000

static void*  __sbrk_base  = NULL;
static size_t __sbrk_pages = 0;
static size_t __sbrk_size  = 0;

void *sbrk(intptr_t increment) {
    ssize_t size_left = (__sbrk_pages * PAGE_SIZE) - __sbrk_size;

    if (increment > size_left) {
        size_t size_needed  = increment - size_left;
        size_t pages_needed = (size_needed + (PAGE_SIZE - 1)) / PAGE_SIZE;

        void *ret;
        int  sys_errno;
        asm volatile (
            "syscall"
            : "=a"(ret), "=d"(sys_errno)
            : "a"(6), "D"(0), "S"(pages_needed)
            : "rcx", "r11"
        );

        if (ret == NULL) {
            errno = sys_errno;
            return (void *)-1;
        }

        __sbrk_pages += pages_needed;
        __sbrk_size  += increment;
        if (__sbrk_base == NULL) {
            __sbrk_base = ret;
        }
    } else if (increment < 0) {
        __sbrk_size -= increment;
    }

    return (void *)((size_t)__sbrk_base + __sbrk_size);
}

int stat(const char *path, struct stat *result) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        return -1;
    }

    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(9), "D"(fd), "S"(result)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    close(fd);
    return ret;
}

int fstat(int fd, struct stat *result) {
	int ret;
	int sys_errno;

	asm volatile (
        "syscall"
		: "=a"(ret), "=d"(sys_errno)
		: "a"(9), "D"(fd), "S"(result)
		: "rcx", "r11"
    );

	if (ret == -1) {
        errno = sys_errno;
        return -1;
    }

	return 0;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp) {
    int ret;
    int sys_errno;

    asm volatile(
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(25), "D"(clock_id), "S"(tp)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return 0;
}

int sched_yield(void) {
    // TODO: Implement this once qword has it, if ever.
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
    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(19), "D"(fds), "S"(0)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
        return -1;
    }

    return 0;
}

pid_t getpid(void) {
    pid_t pid;
    asm volatile (
        "syscall"
        : "=a"(pid)
        : "a"(5)
        : "rcx", "r11", "rdx"
    );
    return pid;
}

pid_t getppid(void) {
    pid_t ppid;
    asm volatile (
        "syscall"
        : "=a"(ppid)
        : "a"(14)
        : "rcx", "r11", "rdx"
    );
    return ppid;
}

pid_t getpgrp(void) {
    pid_t pgid;
    asm volatile (
        "syscall"
        : "=a"(pgid)
        : "a"(38), "D"(0)
        : "rcx", "r11", "rdx"
    );
    return pgid;
}

gid_t getgid(void) {
    // TODO: Implement when qword supports it.
    assert(!"This is a stub");
    return 0;
}

int setgid(gid_t gid) {
    // TODO: Implement when qword supports it.
    (void)gid;
    assert(!"This is a stub");
    return -1;
}

gid_t getegid(void) {
    // TODO: Implement when qword supports it.
    assert(!"This is a stub");
    return 0;
}

uid_t getuid(void) {
    // TODO: Implement when qword supports it.
    assert(!"This is a stub");
    return 0;
}

int setuid(uid_t uid) {
    int ret;
    asm volatile (
        "syscall"
        : "=a" (ret)
        : "a"(39), "D"(uid)
        : "rcx", "r11", "rdx"
    );
    return ret;
}

uid_t geteuid(void) {
    // TODO: Implement when qword supports it.
    assert(!"This is a stub");
    return 0;
}

int execve(const char *path, char *const argv[], char *const envp[]) {
    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(11), "D"(path), "S"(argv), "d"(envp)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
    }

    return ret;
}

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp) {
    asm volatile (
        "syscall"
        :
        : "a"(40), "D"(rqtp->tv_sec + (rqtp->tv_nsec / 1000000000))
        : "rcx", "r11"
    );

    if (rmtp != NULL) {
        rmtp->tv_sec  = 0;
        rmtp->tv_nsec = 0;
    }

    return 0;
}

int ttyname_r(int fd, char *name, size_t namesize) {
    // TODO: Implement when qword supports it.
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
    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(27), "D"(pid), "S"(signal)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
        return -1;
    }

    return 0;
}

int ioctl(int fd, int request, ...) {
    // TODO: Implement when qwordOS supports it.
    (void)fd;
    (void)request;
    assert(!"This is a stub");
    return -1;
}

int tcgetattr(int fd, struct termios *result) {
    int ret;
    int sys_errno;

    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(24), "D"(fd), "S"(result)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
        return -1;
    }

    return 0;
}

int tcsetattr(int fd, int action, const struct termios *result) {
    int ret;
    int sys_errno;
    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(23), "D"(fd), "S"(action), "d"(result)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
        return -1;
    }

    return 0;
}

int tcflow(int fd, int action) {
    int ret;
    int sys_errno;
    asm volatile (
        "syscall"
        : "=a"(ret), "=d"(sys_errno)
        : "a"(30), "D"(fd), "S"(action)
        : "rcx", "r11"
    );

    if (ret == -1) {
        errno = sys_errno;
        return -1;
    }

    return 0;
}

int fchmod(int fd, mode_t mode) {
    // TODO: Implement when qword supports it.
    (void)fd;
    (void)mode;
    assert(!"This is a stub");
    return -1;
}
