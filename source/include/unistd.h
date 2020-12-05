#ifndef __UNISTD_H__
#define __UNISTD_H__

#include <sys/types.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/fcntlval.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char **environ;

int     close(int fd);                                // Implemented by the port.
ssize_t read(int fd, void *buf, size_t count);        // Ditto.
ssize_t write(int fd, const void *buf, size_t count); // Ditto.
char   *getcwd(char *buf, size_t size);               // Ditto.
int     rmdir(const char *path);                      // Ditto.
int     unlink(const char *path);                     // Ditto.
int     fork(void);                                   // Ditto.
void    _exit(int status);                            // Ditto.
off_t   lseek(int fd, off_t offset, int whence);      // Ditto.
void   *sbrk(intptr_t increment);                     // Ditto.
int     access(const char *path, int amode);          // Ditto.
int     pipe(int fds[2]);                             // Ditto.
pid_t   getpid(void);                                 // Ditto.
pid_t   getppid(void);                                // Ditto.
pid_t   getpgrp(void);                                // Ditto.
gid_t   getgid(void);                                 // Ditto.
int     setgid(gid_t gid);                            // Ditto.
gid_t   getegid(void);                                // Ditto.
uid_t   getuid(void);                                 // Ditto.
int     setuid(uid_t uid);                            // Ditto.
uid_t   geteuid(void);                                // Ditto.
int     brk(void *address);

int execl(const char *path, const char *arg0, ...);
int execv(const char *path, char *const argv[]);
int execle(const char *path, const char *arg0, ...);
int execve(const char *path, char *const argv[], char *const envp[]); // Port.
int execlp(const char *file, const char *arg0, ...);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);

#ifdef __cplusplus
}
#endif

#endif
