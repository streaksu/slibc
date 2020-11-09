# slibc, the streaks libc

A POSIX-compatible libc meant for ease of porting and performance for hobbyist
OSes, obtaining this by clean abstractions and elegant C code.

# Porting

To learn about porting slibc to a new OS or architecture please refer to
[this document](PORTING.md).

# Building

A simple makefile is provided, featuring a `PREFIX` and `DESTDIR` variables
for choosing a path for installation.

Only a C toolchain able to compile and link C and ASM and GNU Make are needed
for compiling the project.

An example process would be:

```bash
make CFLAGS="-O2 -pipe"     # Optimization flags are not passed by default.
make test                   # Run the automated tests.
make PREFIX="/usr/" install # Install, feel free to use PREFIX or DESTDIR.
```
