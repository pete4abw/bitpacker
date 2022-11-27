/* Bitpacker POC program
 * compile along with bitpacker.c
 *
 * Usage: bitp [p|u] "ASCII string to pack"
 * where p is pack and encode to base64 ASCII string
 * where u is unpack base64 and decode to ASCII string
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

#define PRINTABLE(p)	p >= 0x20  && p < 0x80 ? p : 0x20	/* Printable Chars only */

/* defined in bitpacker.c library */
extern int bperr;		/* bitpacker error code */
extern char *bperrstr[];	/* bitpacker error strings */

int main (int argc, char *argv[])
{
	BYTE *in = (BYTE *) argv[2];
	BYTE *pack, *unpack;
	BYTE *b64decoded;				/* need this to add \0 because base64_decode
							   may not add a terminating null */
	guchar *base64dec;
	gchar *base64enc;
	gsize b64_decode_len;
	int packlen;

	if (argc != 3)
	{
		fprintf(stderr,"Usage: %s [Command] \"String to pack/unpack\"\n", argv[0]);
		fprintf(stderr,"Command is `p` or `u`\n");
		return 1;
	}
	if (*argv[1] == 'p')
	{
		pack = abitpack(in);
		if (!pack || bperr)			/* somoething wrong */
		{
			fprintf(stderr, "An error %d occurred during abitpack. %s. Aborting\n", bperr, bperrstr[bperr]);
			return 1;
		}
		packlen = strlen((char *) pack);
		base64enc = g_base64_encode(pack, packlen);
		fprintf(stdout,"Base64 encoding of %s is %s\n", in, base64enc);
	}
	else if (*argv[1] == 'u')
	{
		/* first, decode base64 */
		base64dec = g_base64_decode((gchar *) in, &b64_decode_len);
		b64decoded = calloc(b64_decode_len+1, 1);
		if (!b64decoded)
		{
			fprintf(stderr,"Cannot allocate b64decoded\n");
			return -1;
		}
		strncpy((char *) b64decoded, (char *) base64dec, b64_decode_len);
		unpack = abitunpack(b64decoded);
		if (!unpack || bperr)			/* somoething wrong */
		{
			fprintf(stderr, "An error %d occurred during abitunpack. %s. Aborting\n", bperr, bperrstr[bperr]);
			return 1;
		}
		fprintf(stdout,"base64 Decoded packed password is: %s\n", unpack);
	}
	else
	{
		fprintf(stderr,"Invalid argumend. Must be `p` to pack and encode, or `u` to decode and unpack\n");
		return -1;
	}

	return 0;
}
