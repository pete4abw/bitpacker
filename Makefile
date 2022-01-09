# Makefile for bitpacker
# ready for debug

all:
	gcc -O0 -g -Wall  -I./ bptest.c bitpacker.c -o bptest
