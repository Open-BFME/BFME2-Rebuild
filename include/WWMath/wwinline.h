// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// wwinline.h: WWINLINE, Zero Hour's always.h spelling of the forced inline that WWMath's headers use.
#pragma once
#ifndef BFME2_WWINLINE_H
#define BFME2_WWINLINE_H

// evidence: retail 0x007173E0, 0x007176D0 and 0x00712150 expand Sin, Cos and Sqrt at each call site (fld; fsin/fcos/fsqrt; fstp); no plainer form reproduces them.
#define WWINLINE __forceinline

#endif
