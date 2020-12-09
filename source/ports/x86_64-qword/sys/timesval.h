#ifndef __SYS__TIMESVAL_H__
#define __SYS__TIMESVAL_H__

#ifdef __cplusplus
extern "C" {
#endif

struct tms {
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
    clock_t tms_cstime;
};

#ifdef __cplusplus
}
#endif

#endif
