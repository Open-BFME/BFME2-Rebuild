// RGBColor.h: SAGE's three-float colour (Zero Hour BaseType.h RGBColor).
// Target facts: struct (Xfer::operator==(RGBColor&) mangles AAURGBColor);
// XferSaveAsText 0x0060DC50 prints red +0, green +4, blue +8 as "r:,g:,b: [rgb]".
// getAsInt (0x00004EA7, 31 direct callers) and setFromInt (0x00004EDF, 30 direct
// callers) are out of line. The exported constructors are inline: the default one
// folds onto 0x0047A6A9 (`mov eax,ecx; ret`), the three-float one onto Coord3D's
// store body 0x000423A0, and RGBColor(Int) is the COMDAT 0x00006538 that forwards
// to setFromInt.
#pragma once
#ifndef BFME2_RGBCOLOR_H
#define BFME2_RGBCOLOR_H

#include "Common/GameType.h"

struct RGBColor
{
	Real red;		// 0x0
	Real green;		// 0x4
	Real blue;		// 0x8

	RGBColor() {}
	RGBColor(Real r, Real g, Real b) : red(r), green(g), blue(b) {}
	RGBColor(Int color) { setFromInt(color); }

	Int getAsInt() const;
	void setFromInt(Int color) throw();	// retail calls it from ctors with no exception state (FXList.cpp TintDrawableFXNugget 0x001E0986)
};
// SIZE 0xC
typedef char RGBColorSize[(sizeof(RGBColor) == 0xC) ? 1 : -1];

#endif
