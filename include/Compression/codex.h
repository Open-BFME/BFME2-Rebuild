// codex.h: the pieces of EA Canada's CODEX/GIMEX headers the compression modules in Libraries/Source/Compression/EAC include
// (Zero Hour's Compression/EAC/codex.h and gimex.h, GPL-3.0-or-later; see LICENSE and NOTICE).
// Target facts: retail's REF/HUFF/BTREE decoders are C modules with undecorated stdcall names (_REF_decode@12) and keep
// ggetm 0x0068DC60 as an out-of-line copy (eax = source, ecx = byte count) shared by their callers.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later.
#pragma once
#ifndef BFME2_CODEX_H
#define BFME2_CODEX_H

#if !defined(GCALL)
#define GCALL __stdcall
#endif

#ifndef qmin
#define qmin(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef qmax
#define qmax(a,b) ((a)>(b)?(a):(b))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* get motorola memory */
static __inline unsigned int ggetm(const void *src, int bytes)
{
	if (bytes == 1)
		return (unsigned int) *(const unsigned char *) src;
	else if (bytes == 2)
		return (((unsigned int) *(const unsigned char *) src) << 8) | (((unsigned int) *((const unsigned char *) src + 1)));
	else if (bytes == 4)
		return (((unsigned int) *(const unsigned char *) src) << 24) | (((unsigned int) *((const unsigned char *) src + 1)) << 16) | (((unsigned int) *((const unsigned char *) src + 2)) << 8) | (((unsigned int) *((const unsigned char *) src + 3)));
	else if (bytes == 3)
		return (((unsigned int) *(const unsigned char *) src) << 16) | (((unsigned int) *((const unsigned char *) src + 1)) << 8) | (((unsigned int) *((const unsigned char *) src + 2)));
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif
