#include <signal.h>

const int sys_nsig = 32;

const char *const sys_siglist[] = {
    [SIGHUP]    = "Hangup",
    [SIGINT]    = "Terminal interrupt signal",
    [SIGQUIT]   = "Terminal quit signal",
    [SIGILL]    = "Illegal instruction",
    [SIGTRAP]   = "Trace/breakpoint trap",
    [SIGABRT]   = "Process abort signal",
    [SIGBUS]    = "Access to an undefined portion of a memory object",
    [SIGFPE]    = "Erroneous arithmetic operation",
    [SIGKILL]   = "Kill (cannot be caught or ignored)",
    [SIGUSR1]   = "User-defined signal 1",
    [SIGSEGV]   = "Invalid memory reference",
    [SIGUSR2]   = "User-defined signal 2",
    [SIGPIPE]   = "Write on a pipe with no one to read it",
    [SIGTERM]   = "Termination signal",
    [SIGSTKFLT] = "Stack fault",
    [SIGCHLD]   = "Child process terminated, stopped",
    [SIGCONT]   = "Continue executing, if stopped",
    [SIGSTOP]   = "Stop executing (cannot be caught or ignored)",
    [SIGTSTP]   = "Terminal stop signal",
    [SIGTTIN]   = "Background process attempting read",
    [SIGTTOU]   = "Background process attempting write",
    [SIGURG]    = "High bandwidth data is available at a socket",
    [SIGXCPU]   = "CPU time limit exceeded",
    [SIGXFSZ]   = "File size limit exceeded",
    [SIGVTALRM] = "Virtual timer expired",
    [SIGPROF]   = "Profiling timer expired",
    [SIGWINCH]  = "Window size change",
    [SIGPOLL]   = "Pollable event",
    [SIGPWR]    = "Power failure restart",
    [SIGSYS]    = "Bad system call",
    [SIGRTMIN]  = "Real time signal min range",
    [SIGRTMAX]  = "Real time signal max range"
};
