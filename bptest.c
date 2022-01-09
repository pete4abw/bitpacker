/* Bitpacker POC program
 * compile along with bitpacker.c
 *
 * Usage: bptest "Ascii string to pack"
 */

/* This POC is freeware.
 * There is no warranty or performance guarantee.
 * Code may be modified, used, as desired.
 * Attribution to author is appreciated.
 *
 * Copyright 2022 Peter Hyman, pete@peterhyman.com
 */

#include "bitpacker.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define PRINTABLE(p)	p >= 0x20 ? p : 0x20

int main (int argc, char *argv[])
{
	BYTE *in = (BYTE *) argv[1];
	BYTE *out;
	int i,j,il,ol;

	if (argc != 2)
	{
		fprintf(stderr,"Usage: bptest \"String to pack\"\n");
		return 1;
	}
	il=strlen((char *)in);
	out = abitpack(in);
	ol = strlen((char *)out);
	printf("Input length is: %d\nOutput length is: %d\n", il, ol);

	for ( i  =  0; i < il; i++)
	{
		printf("%1c %02X: ", in[i], in[i]);
		for ( j  =  7; j >=  0; j-- )
			printf("%1d ", (bool) (in[i] & (1 << j)));
		printf("\n");
	}
	printf("\n");
	for ( i  =  0; i < ol; i++)
	{
		printf("%1c %02X: ", PRINTABLE(out[i]), out[i]);
		for ( j  =  7; j >=  0; j-- )
			printf("%1d ", (bool) (out[i] & (1 << j)));
		printf("\n");
	}
	printf("\n");

	in = abitunpack(out);
	il = strlen((char *)in);

	for ( i  =  0; i < il; i++)
	{
		printf("%1c %02X: ", PRINTABLE(in[i]), in[i]);
		for ( j  =  7; j >=  0; j-- )
			printf("%1d ", (bool) (in[i] & (1 << j)));
		printf("\n");
	}
	printf("\n");

	return 0;
}
