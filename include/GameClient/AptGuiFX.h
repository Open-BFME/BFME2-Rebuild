// AptGuiFX.h: the Apt GUI effects holder (the tooltip display string and the message boxes). Every member is static: the
// class has no data of its own (the tooltip string, the tooltip level owner and the "initialised" flag are globals).
//
// Target facts (retail game.dat, RVAs): OnInitialized 0x003808E8 sets the byte global the map names g_Va00E022E0 (VA 0x00E022E0);
// Init 0x00380B0C, MoveToolTip 0x003807F3 and ShowToolTip 0x003808F0 are the other statics the map spells as AptGuiFX members.
#pragma once
#ifndef BFME2_APTGUIFX_H
#define BFME2_APTGUIFX_H

class AsciiString;

class AptGuiFX
{
public:
	static void OnInitialized(const char *);							// 0x003808E8: marks the effects initialised
	static void MoveToolTip(float x, float y);							// 0x003807F3
	static void ShowToolTip(void *, AsciiString *, int, int, int);		// 0x003808F0
	static void Init();													// 0x00380B0C
};

#endif
