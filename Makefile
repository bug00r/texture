#MAKE?=mingw32-make
AR?=ar
ARFLAGS?=rcs
PATHSEP?=/
CC=gcc
BUILDROOT?=build

ifeq ($(CLANG),1)
	export CC=clang
endif

BUILDDIR?=$(BUILDROOT)$(PATHSEP)$(CC)
BUILDPATH?=$(BUILDDIR)$(PATHSEP)

INSTALL_ROOT?=$(BUILDPATH)

ifeq ($(DEBUG),1)
	export debug=-ggdb -Ddebug=1
	export isdebug=1
endif

ifeq ($(ANALYSIS),1)
	export analysis=-Danalysis=1
	export isanalysis=1
endif

ifeq ($(DEBUG),2)
	export debug=-ggdb -Ddebug=2
	export isdebug=1
endif

ifeq ($(DEBUG),3)
	export debug=-ggdb -Ddebug=3
	export isdebug=1
endif

ifeq ($(OUTPUT),1)
	export outimg= -Doutput=1
endif

BIT_SUFFIX=

ifeq ($(M32),1)
	CFLAGS+=-m32
	BIT_SUFFIX+=32
endif

CFLAGS+=-std=c11 -Wpedantic -pedantic-errors -Wall -Wextra $(debug)
#-ggdb
#-pg for profiling 

LIB?=-L/c/dev/lib$(BIT_SUFFIX)
INCLUDE?=-I/c/dev/include -I.

SRC=texture.c

INCLUDEDIR=$(INCLUDE) -I.

LIBNAME=libtexture.a
OBJS=$(BUILDPATH)texture.o $(BUILDPATH)texture_cache.o

TESTSRC=test_texture.c
CACHETESTSRC=test_texture_cache.c
TESTBIN=test_texture.exe
CACHETESTBIN=test_texture_cache.exe
TESTLIB=-ltexture -lfractals -lnoise -lcrgb_array -lfarray -ldl_list -larray -lcolor -lstatistics -lutilsmath -lmat -lvec 
TESTLIBDIR=-L$(BUILDDIR) $(LIB)

all: createdir $(BUILDPATH)$(LIBNAME) $(BUILDPATH)$(TESTBIN) $(BUILDPATH)$(CACHETESTBIN)

$(BUILDPATH)$(LIBNAME): $(OBJS)
	$(AR) $(ARFLAGS) $(BUILDPATH)$(LIBNAME) $(OBJS)

$(BUILDPATH)%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDEDIR) 
	
$(BUILDPATH)$(TESTBIN):
	$(CC) $(CFLAGS) $(TESTSRC) -o $(BUILDPATH)$(TESTBIN) $(INCLUDEDIR) $(TESTLIBDIR) $(TESTLIB) $(debug) $(outimg)
	
$(BUILDPATH)$(CACHETESTBIN):
	$(CC) $(CFLAGS) $(CACHETESTSRC) -o $(BUILDPATH)$(CACHETESTBIN) $(INCLUDEDIR) $(TESTLIBDIR) $(TESTLIB) $(debug)

.PHONY: createdir clean test

createdir:
	mkdir -p $(BUILDDIR)

test:
	./$(BUILDPATH)$(TESTBIN)
	./$(BUILDPATH)$(CACHETESTBIN)

clean:
	-rm -dfr $(BUILDROOT)

install:
	mkdir -p $(INSTALL_ROOT)include
	mkdir -p $(INSTALL_ROOT)lib$(BIT_SUFFIX)
	cp ./texture.h $(INSTALL_ROOT)include/texture.h
	cp ./texture_cache.h $(INSTALL_ROOT)include/texture_cache.h
	cp $(BUILDPATH)$(LIBNAME) $(INSTALL_ROOT)lib$(BIT_SUFFIX)/$(LIBNAME)

	