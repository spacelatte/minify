#!/usr/bin/env make -f


AR              := ar
CC              := clang
LD              := ld
LIBS            := -lm -lc
CFLAGS          := -O
LDFLAGS         := -dylib
OUT_LIB_STATIC  := minify.a
OUT_LIB_DYNAMIC := minify.so
OUT_EXE         := minify.exe

default: binary

all: dynamic static binary

static: $(OUT_LIB_STATIC)

dynamic: $(OUT_LIB_DYNAMIC)

binary: $(OUT_EXE)

clean: $(OUT_EXE) $(OUT_LIB_STATIC) $(OUT_LIB_DYNAMIC)
	rm -rf $^ test.*

$(OUT_EXE): $(OUT_LIB_STATIC)
	$(CC) -o $@ $< main.c

%.so %.dylib: %.o
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $<

%.a: %.o
	$(AR) -rsuv $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

test.%: $(OUT_EXE)
	dd if=/dev/zero bs=1024 count=1000 | tr \\0 $* > $@
	shasum $@
	./$(OUT_EXE) $@ $@.min
	shasum $@

test.0: $(addprefix test., 1 2 3 4 5 6 7 8 9)
	cat $^ > $@
	shasum $@
	./$(OUT_EXE) $@ $@.min
	shasum $@

test: test.0
	true
