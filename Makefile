# Makefile for bitpacker
# ready for debug

# will compile a bitpacker shared library and link it to bptest program
#

GLIB=`pkg-config --cflags --libs glib-2.0`

all: bitpacker.o bptest bitp

bitpacker.o: bitpacker.c bitpacker.h
	# compile bitpacker library
	@echo "creating libbitpacker library"
	gcc -O0 -g -Wall -I./ -fPIC -c bitpacker.c
	# make into shared library for other uses
	gcc -shared -o libbitpacker.so bitpacker.o

bptest: bptest.c bitpacker.c bitpacker.h
	# link statically to avoid having to use shared library
	@echo "creating bptest file"
	gcc -O0 -g -Wall $(GLIB)  -I ./  bptest.c -o bptest bitpacker.o

bitp: bitp.c bitpacker.c bitpacker.h
	@echo "creating bitp program"
	gcc -O0 -g -Wall $(GLIB)  -I ./  bitp.c -o bitp bitpacker.o

clean:
	rm -v bptest bitp *.o *.so test.pack
