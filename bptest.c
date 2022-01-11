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

#define PRINTABLE(p)	p >= 0x20  && p < 0x7F ? p : 0x20	/* Printable Chars only */

int main (int argc, char *argv[])
{
	BYTE *in = (BYTE *) argv[1];
	BYTE *pack, *unpack;
	int i,j,il,pl;

	if (argc != 2)
	{
		fprintf(stderr,"Usage: bptest \"String to pack\"\n");
		return 1;
	}
	il=strlen((char *)in);
	pack = abitpack(in);
	pl = strlen((char *)pack);
	unpack = abitunpack(pack);	/* should be same as input length */
	printf("Input length is: %d\nOutput length is: %d\n", il, pl);

	for ( i  =  0; i < il; i++)
	{
		printf("%1c %02X: ", PRINTABLE(in[i]), in[i]);
		for ( j  =  7; j >=  0; j-- )
			printf("%1d ", (bool) (in[i] & (1 << j)));
		printf("    ");
		if (i < pl)		/* print output if we can */
		{
			printf("%1c %02X: ", PRINTABLE(pack[i]), pack[i]);
			for ( j  =  7; j >=  0; j-- )
				printf("%1d ", (bool) (pack[i] & (1 << j)));
		}
		else
			printf("- --: - - - - - - - - ");
		printf("    ");
		printf("%1c %02X: ", PRINTABLE(unpack[i]), unpack[i]);
		for ( j  =  7; j >=  0; j-- )
			printf("%1d ", (bool) (unpack[i] & (1 << j)));

		printf("\n");
	}

	FILE *fp;
	fp = fopen("test.pack","w+");
	if (fp == NULL)
		return 1;
	i = fwrite(pack, 1, pl, fp);
	fclose(fp);
	printf("\n%d bytes written to file \"test.pack\". %d expected.\n", i, pl);
	return 0;
}
