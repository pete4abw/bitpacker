/* bitpacker
 * pack a stream of 7-bit ascii into 8 bit bytes
 * return pointer to packed stream
 * unpack a stream of 8 bit bytes
 * return pointer to unpacked stream
 * return NULL on any problem
*/

/* This POC is freeware.
 * There is no warranty or performance guarantee.
 * Code may be modified, used, as desired.
 * Attribution to author is appreciated.
 *
 * Copyright 2022 Peter Hyman, pete@peterhyman.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "bitpacker.h"

/* used by functions */
#define SHL(p,n)	(p << n)
#define SHR(p,n)	(p >> n)
#define MASKBITS(p,n)	(!n ? p & 1 : p & ((2 << n) - 1))

int bperr;		/* global error */
char *bperrstr[]={"bitpacker: OK", 
	"bitpacker: No Input to (un)pack", 
	"bitpacker: Input not 7 bit ASCII", 
	"bitpacker: FATAL! Cannot allocate ram for (un)pack"};	/* global error strings */

BYTE * abitpack(const BYTE *in)
{
	int outpos = 0, inpos = 0, prevbyte = 0, bitpos = 0;
	int length;
	BYTE *out;

	if (!in)			/* nothing passed */
	{
		bperr=BPENOIN;
		return NULL;
	}

	length = strlen((char *)in);
	if (length <= 0)		/* 0 length or strlen error */
	{
		bperr=BPENOIN;
		return NULL;
	}

	out=calloc(length, 1);		/* allocate output string */
	if (out == NULL)		/* something very wrong */
	{
		bperr=BPENOOUT;
		return NULL;
	}
	length--;			/* make length 0-based */

	do
	{
		if (in[inpos] & 0b10000000)			/* can't bitpack because high order bit set */
		{
			bperr=BPE8BIT;
			free(out);				/* cleanup */
			return NULL;
		}

		out[outpos] = SHL(in[inpos], (bitpos + 1));	/* shift current byte 1-7 bits */
		if (bitpos > 0)					/* if second or more time through, update prior byte */
		{
			out[prevbyte] |= SHR(in[inpos], (7 - bitpos));	/* with bit 6 or more */
		}
		if (bitpos == 6)				/* unless it's the 7th byte or multiple */
		{
			if (inpos <= length)
				out[outpos] |= in[++inpos];
			bitpos = -1;				/* reset */
		}
		prevbyte = outpos;
		outpos++;
		inpos++;
		bitpos++;
	} while (inpos <= length);
	bperr=BPEOK;
	return (out);
}

BYTE * abitunpack(const BYTE *in)
{
	int outpos = 0, inpos = 0, bitpos = 0;
	int length, outlength;
	BYTE work;
	BYTE *out;

	if (!in)			/* nothing passed */
	{
		bperr=BPENOIN;
		return NULL;
	}

	length = strlen((char *)in);		/* packed input length */
	if (length <= 0)			/* 0 length or strlen error */
	{
		bperr=BPENOIN;
		return NULL;
	}
	/* to determine outlength for fewer than 8 bytes, we must look at
	 * the value of byte 7 or its multiples.
	 * if the value of byte 7, 14, 21, etc. is 0 or not If 0
	 * then there are 7 bytes to unpack. Any other value,
	 * is a byte 8 or multiple to unpack too.
	 */

	outlength = length;
	if (length > 7)
		outlength += (int) (length / 7);	/* pad outlength accordingly */

	if (length % 7)					/* we may have an extra byte */
	{
		if (in[length-1] & 0b01111111)		/* there's something in last packed byte */
			++outlength;
	}
	length--;			/* 0-based */

	out=calloc(outlength, 1);
	if (!out)			/* something very wrong */
	{
		bperr=BPENOOUT;
		return NULL;
	}
	outlength--;			/* 0-based */

	do
	{
		work = MASKBITS(in[inpos], bitpos);		/* save high bit(s) of next byte */
		out[outpos] |= SHR(in[inpos], (bitpos + 1));	/* shift current byte 1-7 bits */

		if (bitpos < 6)					/* update next byte */
		{
			if (outpos < outlength)
				out[outpos+1] |= SHL(work, (6 - bitpos));	/* paste high order bits into next byte */
		}
		else if (bitpos == 6)				/* unless it's the 7th byte or multiple */
		{
			if (inpos <= length)			/* more to come? */
				out[++outpos] = work;		/* incrrement outpos and set for next 7 bytes of unpack */
			bitpos = -1;				/* reset */
		}
		outpos++;
		inpos++;
		bitpos++;
	} while (inpos <= length);
	bperr=BPEOK;
	return (out);
}
