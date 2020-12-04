#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <sched.h>

#define LINUX_SYSCALL(ret, syscall) do {                             \
    __asm__ __volatile__ (                                           \
        "syscall": "=a"(ret) : "a"(syscall) : "rcx", "r11", "memory" \
    );                                                               \
} while (0)

#define LINUX_SYSCALL1(ret, syscall, arg1) do {                                 \
    __asm__ __volatile__ (                                                      \
        "syscall": "=a"(ret) : "a"(syscall), "D"(arg1) : "rcx", "r11", "memory" \
    );                                                                          \
} while (0)

#define LINUX_SYSCALL1_NORET(syscall, arg1) do {                      \
    __asm__ __volatile__ (                                            \
        "syscall": : "a"(syscall), "D"(arg1) : "rcx", "r11", "memory" \
    );                                                                \
} while (0)

#define LINUX_SYSCALL2(ret, syscall, arg1, arg2) do {             \
    __asm__ __volatile__ (                                        \
        "syscall": "=a"(ret) : "a"(syscall), "D"(arg1), "S"(arg2) \
        : "rcx", "r11", "memory"                                  \
    );                                                            \
} while (0)

#define LINUX_SYSCALL3(ret, syscall, arg1, arg2, arg3) do {                  \
    __asm__ __volatile__ (                                                   \
        "syscall": "=a"(ret) : "a"(syscall), "D"(arg1), "S"(arg2), "d"(arg3) \
        : "rcx", "r11", "memory"                                             \
    );                                                                       \
} while (0)

int fcntl(int fd, int command, ...) {
    va_list args;
    va_start(args, command);
    uint64_t arg = va_arg(args, uint64_t);

    int ret;
    LINUX_SYSCALL3(ret, 72, fd, command, arg);

    if (ret < 0) {
        errno = -ret;
        va_end(args);
        return -1;
    }

    va_end(args);
    return ret;
}

int open(const char *path, int oflag, ...) {
    va_list args;
    va_start(args, oflag);

    mode_t mode = 0;
    if (oflag & (O_CREAT | O_TMPFILE)) {
        mode = va_arg(args, mode_t);
    }

    ssize_t ret;
    LINUX_SYSCALL3(ret, 2, path, oflag, mode);

    if (ret < 0) {
        errno = -ret;
        va_end(args);
        return -1;
    }

    va_end(args);
    return ret;
}

int close(int fd) {
    ssize_t ret;
    LINUX_SYSCALL1(ret, 3, fd);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}

ssize_t read(int fd, void *buf, size_t count) {
    ssize_t ret;
    LINUX_SYSCALL3(ret, 0, fd, buf, count);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}

ssize_t write(int fd, const void *buf, size_t count) {
    ssize_t ret;
    LINUX_SYSCALL3(ret, 1, fd, buf, count);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}

char *getcwd(char *buf, size_t size) {
    ssize_t ret;
    LINUX_SYSCALL2(ret, 79, buf, size);

    if (ret < 0) {
        errno = -ret;
        return NULL;
    }

    return buf;
}

int rmdir(const char *path) {
    ssize_t ret;
    LINUX_SYSCALL1(ret, 84, path);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return 0;
}

int unlink(const char *path) {
    ssize_t ret;
    LINUX_SYSCALL1(ret, 87, path);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return 0;
}

pid_t fork(void) {
    pid_t ret;
    LINUX_SYSCALL(ret, 57);

    if (ret < 0) {
        errno = -ret;
    }

    return ret;
}

void _exit(int error_code) {
    LINUX_SYSCALL1_NORET(60, error_code);
}

off_t lseek(int fd, off_t offset, int whence) {
    off_t ret;
    LINUX_SYSCALL3(ret, 8, fd, offset, whence);

    if (ret < 0) {
        errno = -ret;
    }

    return ret;
}

static intptr_t __base_brk = -1;

void *sbrk(intptr_t increment) {
    intptr_t current_brk;
    LINUX_SYSCALL1(current_brk, 12, NULL);

    if (__base_brk == -1) {
        __base_brk = current_brk;
    }

    intptr_t new_brk = current_brk + increment;
    if (new_brk < __base_brk) {
        errno = ENOMEM;
        return (void*)-1;
    }

    int ret;
    LINUX_SYSCALL1(ret, 12, new_brk);
    if (ret < 0) {
        errno = -ret;
        return (void*)-1;
    }

    return (void*)current_brk;
}

int stat(const char *path, struct stat *result) {
    int ret;
    LINUX_SYSCALL2(ret, 4, path, result);

    if (ret < 0) {
        errno = -ret;
    }

    return ret;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp) {
    int ret;
    LINUX_SYSCALL2(ret, 228, clock_id, tp);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return 0;
}

int sched_yield(void) {
    int ret;
    LINUX_SYSCALL(ret, 24);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return 0;
}

int access(const char *path, int amode) {
    int ret;
    LINUX_SYSCALL2(ret, 24, path, amode);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return 0;
}

pid_t getpid(void) {
    pid_t ret;
    LINUX_SYSCALL(ret, 39);
    return ret;
}

pid_t getppid(void) {
    pid_t ret;
    LINUX_SYSCALL(ret, 110);
    return ret;
}

pid_t getpgrp(void) {
    pid_t ret;
    LINUX_SYSCALL(ret, 111);
    return ret;
}

gid_t getgid(void) {
    gid_t ret;
    LINUX_SYSCALL(ret, 104);
    return ret;
}

gid_t getegid(void) {
    gid_t ret;
    LINUX_SYSCALL(ret, 108);
    return ret;
}

uid_t getuid(void) {
    uid_t ret;
    LINUX_SYSCALL(ret, 102);
    return ret;
}

int setuid(uid_t uid) {
    int ret;
    LINUX_SYSCALL1(ret, 105, uid);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return 0;
}

uid_t geteuid(void) {
    uid_t ret;
    LINUX_SYSCALL(ret, 107);
    return ret;
}

int execve(const char *path, char *const argv[], char *const envp[]) {
    int ret;
    LINUX_SYSCALL3(ret, 59, path, argv, envp);

    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}
