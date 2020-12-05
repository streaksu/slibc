# Porting slibc

To get the changes on tree please make a pull request, I can mantain and
update them from there if needed.

The ports to slibc are located in [source/ports], there, each port targets both
an architecture and OS, for example, `aarch64-linux`. This target can be chosen
for compilation with the `TARGET` Makefile variable.

There, in the chosen folder under [source/ports] a few items must be defined.

# Items to define

Given that we are trying to port to `X-Y`, this are the items needed for a port
and their locations:

- `crt0.S` under `source/ports/X-Y/crt0.S`, which sets up arguments to call
    the `__slibc_init` symbol, with the signature
    `void __slibc_init(int argc, char *argv[], char *envp[])`.
- A series of C files implementing all the required functions, which are:
    - POSIX-compatible `fcntl`.
    - POSIX-compatible `open`.
    - POSIX-compatible `close`.
    - POSIX-compatible `read`.
    - POSIX-compatible `write`.
    - POSIX-compatible `rmdir`.
    - POSIX-compatible `unlink`.
    - POSIX-compatible `getcwd`.
    - POSIX-compatible `fork`.
    - POSIX-compatible `_exit`.
    - POSIX-compatible `lseek`.
    - POSIX-compatible `sbrk`.
    - POSIX-compatible `stat`.
    - POSIX-compatible `fstat`.
    - POSIX-compatible `clock_gettime`.
    - POSIX-compatible `sched_yield`.
    - POSIX-compatible `access`.
    - POSIX-compatible `pipe`.
    - POSIX-compatible `getpid`.
    - POSIX-compatible `getppid`.
    - POSIX-compatible `getpgrp`.
    - POSIX-compatible `getgid`. 
    - POSIX-compatible `setgid`.
    - POSIX-compatible `getegid`.
    - POSIX-compatible `getuid`. 
    - POSIX-compatible `setuid`.
    - POSIX-compatible `geteuid`.
    - POSIX-compatible `execve`.
    - POSIX-compatible `nanosleep`.
    - POSIX-compatible `isatty`.
    - POSIX-compatible `ttyname_r`.
    - POSIX-compatible `alarm`.
    - POSIX-compatible `kill`.
    - All the desired optional functions.
- A declaration of `sys_siglist`.

The functions being syscalls or not does not matter as long as they express
POSIX behaviour, this could be used to emulate POSIX behaviour on a non-POSIX
syscall interface, for example.

- A `sys` directory under `source/ports/X-Y/sys`, which will be installed
verbatim with the final instalation under `sys`, containing:
    - The definition of the `errnoval.h`, `fcntlval.h`, `timeval.h`, `statval.h
     and `signalval.h` headers, which define values taken by fcntl, errno, time
    functions, stat, and signals, by nature OS dependent.
    - The `types` header, since the types and values are part of the OS ABI.
    - Any desired optional headers. for optional declarations or values. 

For a more practical example, the `x86_64-linux` is a complete documented
port that can be used as reference.

# Expected environment

A few conditions are expected by the libc for its inner workings:

- File descriptors `0`, `1` and `2` must be already opened as they will serve as
`stdin`, `stdout` and `stderr` respectively.
