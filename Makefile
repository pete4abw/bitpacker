# Makefile for bitpacker
# ready for debug

# will compile a bitpacker shared library and link it to bptest program
#
.DEFAULT_GOAL := bptest


bptest:
	gcc -O0 -g -Wall -I./ -fPIC -c bitpacker.c
	gcc -shared -o libbitpacker.so bitpacker.o
	gcc -O0 -g -Wall  -I./ -L./ bptest.c  -o bptest -lbitpacker

clean:
	rm -v bptest *.o *.so test.pack
