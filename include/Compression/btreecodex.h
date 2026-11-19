// btreecodex.h: EA Canada's BTREE codex declarations (Zero Hour's Compression/EAC/btreecodex.h, GPL-3.0-or-later).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc.
// Copyright (C) Electronic Arts Canada Inc. 1995-2002.  All rights reserved.
#pragma once
#ifndef BFME2_BTREECODEX_H
#define BFME2_BTREECODEX_H

#include "Compression/codex.h"

#ifdef __cplusplus
extern "C" {
#endif

bool GCALL BTREE_is(const void *compresseddata);
int  GCALL BTREE_size(const void *compresseddata);
int  GCALL BTREE_decode(void *dest, const void *compresseddata, int *compressedsize);
int  GCALL BTREE_encode(void *compresseddata, const void *source, int sourcesize, int *opts = 0);

#ifdef __cplusplus
}
#endif

// The decoder's working state: the clue table, the two child tables and the output cursor.
struct BTreeDecodeContext
{
    signed char    cluetbl[256];
    unsigned char  left[256];
    unsigned char  right[256];
    unsigned char  *d;
};

// The encoder's working state (btreeencode.cpp).
#define BTREECODES	 256

struct BTREEMemStruct
{
	char	*ptr;
	int	len;
};

struct BTreeEncodeContext
{
    unsigned int	packbits;
    unsigned int	workpattern;
    unsigned char  *bufptr;
    unsigned int	ulen;
    unsigned int	masks[17];
    unsigned char	clueq[BTREECODES];
    unsigned char	right[BTREECODES];
    unsigned char	join[BTREECODES];
    unsigned int	plen;
    unsigned char  *bufbase;
    unsigned char  *bufend;
    unsigned char  *buffer;
    unsigned char  *buf1;
    unsigned char  *buf2;
};

#endif
