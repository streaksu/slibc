#ifndef __PWD_H__
#define __PWD_H__

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct passwd {
    char *pw_name;  // User's login name. 
    uid_t pw_uid;   // Numerical user ID. 
    gid_t pw_gid;   // Numerical group ID. 
    char *pw_dir;   // Initial working directory. 
    char *pw_shell; // Program to use as shell.
};

struct passwd *getpwnam(const char *name);
struct passwd *getpwuid(uid_t uid);

int getpwnam_r(const char *, struct passwd *, char *, size_t, struct passwd **);
int getpwuid_r(uid_t, struct passwd *, char *, size_t, struct passwd **);

void           endpwent(void);
struct passwd *getpwent(void);
void           setpwent(void);

#ifdef __cplusplus
}
#endif

#endif
