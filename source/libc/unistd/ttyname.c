#include <unistd.h>

static char name[TTY_NAME_MAX];

char *ttyname(int fd) {
    if (ttyname_r(fd, name, TTY_NAME_MAX) != 0) {
        return NULL;
    }

    return name;
}
