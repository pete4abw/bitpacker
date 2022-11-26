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
#include <glib-2.0/glib.h>

#define PRINTABLE(p)	p >= 0x20  && p < 0x7F ? p : 0x20	/* Printable Chars only */

/* defined in bitpacker.c library */
extern int bperr;		/* bitpacker error code */
extern char *bperrstr[];	/* bitpacker error strings */

int main (int argc, char *argv[])
{
	BYTE *in = (BYTE *) argv[1];
	BYTE *pack, *unpack;
	guchar *ibase64;
	gchar *obase64;
	gsize b64_decode_len;


	int i,j,il,pl,ul;

	if (argc != 2)
	{
		fprintf(stderr,"Usage: bptest \"String to pack\"\n");
		return 1;
	}
	il=strlen((char *)in);
	pack = abitpack(in);
	printf("BPERR=%d, %s\n", bperr, bperrstr[bperr]);
	if (!pack || bperr)			/* somoething wrong */
	{
		fprintf(stderr, "An error %d occurred during abitpack. %s. Aborting\n", bperr, bperrstr[bperr]);
		return 1;
	}
	pl = strlen((char *)pack);
	unpack = abitunpack(pack);	/* should be same as input length */
	printf("BPERR=%d, %s\n", bperr, bperrstr[bperr]);
	if (!unpack || bperr)			/* somoething wrong */
	{
		fprintf(stderr, "An error %d occurred during abitunpack. %s. Aborting\n", bperr, bperrstr[bperr]);
		return 1;
	}
	ul = strlen((char *)unpack);
	printf("Input length is:  %3d\nOutput length is: %3d\nUnpack length is: %3d %s\n\n", il, pl, ul, ul != il ? "Warning Unpack Length different!" : "");

	printf("Char: 7 6 5 4 3 2 1 0     Char: 7 6 5 4 3 2 1 0     Char: 7 6 5 4 3 2 1 0\n");
	printf("---------------------     ---------------------     ---------------------\n");
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

	/* now encode into base64 */
	obase64 = g_base64_encode(pack, pl);
	printf("base64: %s\n", obase64);
	ibase64 = g_base64_decode(obase64, &b64_decode_len);

	if (strcmp((char*) ibase64, (char *) pack))
		printf("Error, base64 decode and pack are not equal\n");
	else
		printf("base64 encoding successful\n");

	FILE *fp;
	fp = fopen("test.pack","w+");
	if (fp == NULL)
		return 1;
	i = fwrite(pack, 1, pl, fp);
	fclose(fp);
	printf("\n%d bytes written to file \"test.pack\". %d expected.\n", i, pl);
	return 0;
}
