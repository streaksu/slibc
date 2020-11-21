#include <time.h>

time_t time(time_t *store) {
    struct timespec t;
    if (clock_gettime(CLOCK_REALTIME, &t)) {
        return (time_t)-1;
    }

    if (store != NULL) {
        *store = t.tv_sec;
    }
    return t.tv_sec;
}
