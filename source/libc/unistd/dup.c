#include <unistd.h>
#include <fcntl.h>

int dup(int fd) {
    return fcntl(fd, F_DUPFD, 0);
}

int dup2(int fd, int fd2) {
    close(fd2);
    return fcntl(fd, F_DUPFD, fd2);
}
