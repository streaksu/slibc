# Locations and targets.
TARGET  = x86_64-linux
PREFIX  = /usr/local
DESTDIR =
SRCDIR := source
INCDIR := source/include
TSTDIR := test
PRTDIR := source/ports/$(TARGET)

# Compilers and its flags.
CC      = cc
AS      = cc
AR      = ar
CFLAGS  = -Wall -Wextra -Wpedantic -g
ASFLAGS =
ARFLAGS =

CHARDFLAGS  := $(CFLAGS) -ffreestanding -I$(INCDIR) -I$(PRTDIR)
ASHARDFLAGS := $(ASFLAGS) -ffreestanding
ARHARDFLAGS := $(ARFLAGS) rcs

# Source to compile.
LIBCSRC := $(shell find $(SRCDIR)/libc -name '*.c')
MATHSRC := $(shell find $(SRCDIR)/libm -name '*.c')
PORTSRC := $(shell find $(PRTDIR)      -name '*.c')
CRT0SRC := $(PRTDIR)/crt0.S

LIBCO := $(LIBCSRC:.c=.o) $(PORTSRC:.c=.o)
MATHO := $(MATHSRC:.c=.o)

CRT0 := crt0.o
LIBC := libc.a
MATH := libm.a

.PHONY: all install-headers install test clean

all: $(CRT0) $(LIBC) $(MATH)

$(CRT0): $(CRT0SRC)
	$(AS) $(ASHARDFLAGS) -c $< -o $@

$(LIBC): $(LIBCO)
	$(AR) $(ARHARDFLAGS) $@ $(LIBCO)

$(MATH): $(MATHO)
	$(AR) $(ARHARDFLAGS) $@ $(MATHO)

%.o: %.c
	$(CC) $(CHARDFLAGS) -c $< -o $@

install-headers:
	install -d "$(DESTDIR)$(PREFIX)/include/sys"
	cp -rL $(INCDIR)/*     "$(DESTDIR)$(PREFIX)/include"
	cp -rL $(PRTDIR)/sys/* "$(DESTDIR)$(PREFIX)/include/sys"

install: install-headers all
	install -d "$(DESTDIR)$(PREFIX)/lib"
	install -m 644 crt0.o "$(DESTDIR)$(PREFIX)/lib/"
	install -m 644 libc.a "$(DESTDIR)$(PREFIX)/lib/"
	install -m 644 libm.a "$(DESTDIR)$(PREFIX)/lib/"

test: all
	echo "Testing libc..."
	$(CC) -g -Wall -ffreestanding -nostdlib -nostartfiles -static $(TSTDIR)/libc.c crt0.o libc.a -I$(INCDIR) -I$(PRTDIR) -o libctest
	./libctest
	rm -rf libctest

clean:
	rm -f $(CRT0) $(LIBC) $(MATH) $(LIBCO) $(MATHO)
