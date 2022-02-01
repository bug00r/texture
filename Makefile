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

CFLAGS+=-std=c11 -Wpedantic -pedantic-errors -Wall -Wextra $(debug) $(outimg)
#-ggdb
#-pg for profiling 


INCLUDE?=-I/c/dev/include -I.

NAME=texture

LIBNAME=lib$(NAME).a
LIB=$(BUILDPATH)$(LIBNAME)

OBJS=$(BUILDPATH)texture.o $(BUILDPATH)texture_cache.o

TESTSRC=test_$(NAME).c
CACHETESTSRC=test_$(NAME)_cache.c
TESTBIN=$(BUILDPATH)test_$(NAME).exe
CACHETESTBIN=$(BUILDPATH)test_$(NAME)_cache.exe
TESTLIB=-l$(NAME) -lfractals -lnoise -lcrgb_array -lfarray -ldl_list -larray -lcolor -lstatistics -lutilsmath -lmat -lvec 
TESTLIBDIR=-L$(BUILDDIR) -L/c/dev/lib$(BIT_SUFFIX)

all: createdir $(LIB) $(TESTBIN) $(CACHETESTBIN)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJS):
	$(CC) $(CFLAGS) -c $(@F:.o=.c) -o $@ $(INCLUDE) 
	
$(TESTBIN):
	$(CC) $(CFLAGS) $(@F:.exe=.c) -o $@ $(INCLUDE) $(TESTLIBDIR) $(TESTLIB) 
	
$(CACHETESTBIN):
	$(CC) $(CFLAGS) $(@F:.exe=.c) -o $@ $(INCLUDE) $(TESTLIBDIR) $(TESTLIB)

.PHONY: createdir clean test

createdir:
	mkdir -p $(BUILDDIR)

test:
	./$(TESTBIN)
	./$(CACHETESTBIN)

clean:
	-rm -dfr $(BUILDROOT)

install:
	mkdir -p $(INSTALL_ROOT)include
	mkdir -p $(INSTALL_ROOT)lib$(BIT_SUFFIX)
	cp ./$(NAME).h $(INSTALL_ROOT)include$(PATHSEP)$(NAME).h
	cp ./$(NAME)_cache.h $(INSTALL_ROOT)include$(PATHSEP)$(NAME)_cache.h
	cp $(LIB) $(INSTALL_ROOT)lib$(BIT_SUFFIX)$(PATHSEP)$(LIBNAME)

	