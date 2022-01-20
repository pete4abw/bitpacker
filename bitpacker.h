/* bitpacker.h
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

#define BPEOK    0	/* all OK */
#define BPENOIN  1	/* No input provided */
#define BPE8BIT  2	/* 8 bit ASCII detected */
#define BPENOOUT 3	/* this is very bad! */

typedef unsigned char	BYTE;

/* Pack 7-bit ASCII string
 * Returns allocated pointer to packed string
 * Returns NULL on any error */
BYTE * abitpack(const BYTE *in);

/* Unpack 8-bit character string into 7-bit ASCII string
 * Takes input string and returns allocated pointer to
 * output string and ready to go
 * Returns NULL on any error */
BYTE * abitunpack(const BYTE *in); 
