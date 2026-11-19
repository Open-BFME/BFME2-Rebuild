// refcodex.h: EA Canada's REF (RefPack) codex declarations (Zero Hour's Compression/EAC/refcodex.h, GPL-3.0-or-later).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc.
// Copyright (C) Electronic Arts Canada Inc. 1995-2002.  All rights reserved.
#pragma once
#ifndef BFME2_REFCODEX_H
#define BFME2_REFCODEX_H

#include "Compression/codex.h"

#ifdef __cplusplus
extern "C" {
#endif

bool GCALL REF_is(const void *compresseddata);
int  GCALL REF_size(const void *compresseddata);
int  GCALL REF_decode(void *dest, const void *compresseddata, int *compressedsize = 0);
int  GCALL REF_encode(void *compresseddata, const void *source, int sourcesize, int *opts = 0);

#ifdef __cplusplus
}
#endif

#endif
