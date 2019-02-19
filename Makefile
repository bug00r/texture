include ../make_config

CFLAGS=-std=c11 -Wpedantic -pedantic-errors -Wall -Wextra -O1 $(debug)
#-ggdb 
#-pg for profiling 

SRC=texture.c

INCLUDEDIR=-I./../math/algorithm/fractals/ -I./../math/algorithm/noise/ -I./../collections/array/ -I./../color -I./../math/statistics -I./../math/vec -I./../math/mat -I./../math/utils -I.

LIBNAME=libtexture.a
OBJS=texture.o

TESTSRC=test_texture.c
TESTBIN=test_texture.exe
TESTLIB=-ltexture -lfractals -lnoise -lcrgb_array -lfarray -larray -lcolor -lstatistics -lutilsmath -lmat -lvec 
TESTLIBDIR=-L$(BUILDDIR) \
			-L./../math/algorithm/fractals/$(BUILDDIR) \
			-L./../math/algorithm/noise/$(BUILDDIR) \
			-L./../collections/array/$(BUILDDIR) \
			-L./../color/$(BUILDDIR) \
			-L./../math/statistics/$(BUILDDIR) \
			-L./../math/utils/$(BUILDDIR) \
			-L./../math/mat/$(BUILDDIR) \
			-L./../math/vec/$(BUILDDIR)

all: createdir $(BUILDPATH)$(LIBNAME) $(BUILDPATH)$(TESTBIN)

$(BUILDPATH)$(LIBNAME): $(BUILDPATH)$(OBJS)
	$(AR) $(ARFLAGS) $(BUILDPATH)$(LIBNAME) $(BUILDPATH)$(OBJS)

$(BUILDPATH)$(OBJS):
	$(CC) $(CFLAGS) -c $(SRC) -o $(BUILDPATH)$(OBJS) $(INCLUDEDIR) 
	
$(BUILDPATH)$(TESTBIN):
	$(CC) $(CFLAGS) $(TESTSRC) -o $(BUILDPATH)$(TESTBIN) $(INCLUDEDIR) $(TESTLIBDIR) $(TESTLIB) $(debug) $(outimg)
	

.PHONY: createdir clean test

createdir:
	mkdir -p $(BUILDDIR)

test:
	./$(BUILDPATH)$(TESTBIN)

clean:
	-rm -dfr $(BUILDROOT)
	