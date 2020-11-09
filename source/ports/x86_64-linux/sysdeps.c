#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

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

int fork(void) {
    ssize_t ret;
    LINUX_SYSCALL(ret, 57);

    if (ret < 0) {
        errno = -ret;
    }

    return ret;
}

void _exit(int error_code) {
    LINUX_SYSCALL1_NORET(60, error_code);
}
