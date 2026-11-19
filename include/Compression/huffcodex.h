// huffcodex.h: EA Canada's HUFF codex declarations (Zero Hour's Compression/EAC/huffcodex.h, GPL-3.0-or-later).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc.
// Copyright (C) Electronic Arts Canada Inc. 1995-2002.  All rights reserved.
#pragma once
#ifndef BFME2_HUFFCODEX_H
#define BFME2_HUFFCODEX_H

#include "Compression/codex.h"

#ifdef __cplusplus
extern "C" {
#endif

bool GCALL HUFF_is(const void *compresseddata);
int  GCALL HUFF_size(const void *compresseddata);
int  GCALL HUFF_decode(void *dest, const void *compresseddata, int *compressedsize = 0);
int  GCALL HUFF_encode(void *compresseddata, const void *source, int sourcesize, int *opts = 0);

#ifdef __cplusplus
}
#endif

// The encoder's working state (huffencode.cpp).
struct HUFFMemStruct
{
	char	*ptr;
	int	len;
};

#define HUFFBIGNUM					32000
#define HUFFTREESIZE				520
#define HUFFCODES					256
#define HUFFMAXBITS					16
#define HUFFREPTBL					252

struct HuffEncodeContext
{
    char            qleapcode[HUFFCODES];
    unsigned int	count[768];
    unsigned int	bitnum[HUFFMAXBITS+1];
    unsigned int	repbits[HUFFREPTBL];
    unsigned int	repbase[HUFFREPTBL];
    unsigned int	tree_left[HUFFTREESIZE];
    unsigned int	tree_right[HUFFTREESIZE];
    unsigned int	bitsarray[HUFFCODES];
    unsigned int	patternarray[HUFFCODES];
    unsigned int	masks[17];
    unsigned int	packbits;
    unsigned int	workpattern;
    unsigned char	*buffer;
    unsigned char	*bufptr;
    int			flen;
    unsigned int	csum;
    unsigned int	mostbits;
    unsigned int	codes;
    unsigned int	chainused;
    unsigned int	clue;
    unsigned int	dclue;
    unsigned int	clues;
    unsigned int	dclues;
    int				mindelta;
    int				maxdelta;
    unsigned int	plen;
    unsigned int	ulen;
    unsigned int	sortptr[HUFFCODES];
};


#endif
