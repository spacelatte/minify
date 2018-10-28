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

clean:
	rm -rf $(OUT_EXE) $(OUT_LIB_STATIC) $(OUT_LIB_DYNAMIC)

$(OUT_EXE): $(OUT_LIB_STATIC)
	$(CC) -o $@ $< main.c

%.so %.dylib: %.o
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $<

%.a: %.o
	$(AR) -rsuv $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

