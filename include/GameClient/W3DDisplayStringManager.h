// W3DDisplayStringManager.h: the Direct3D implementation of DisplayStringManager
// (GameClient/DisplayStringManager.h); allocates W3DDisplayStrings.
//
// Evidence (retail game.dat): constructor 0x00090360 runs the base constructor, stores table
// 0x007C7E20 (18 slots: reset 0x00090430, update 0x000903B0, newDisplayString 0x000905F0,
// freeDisplayString 0x00090381, getGroupNumeralString 0x00090417, the formation-letter getter
// folded with GameWindow::winGetStyle's 4-byte body 0x005C4AF1 returning +0x3C), zeroes the ten
// words from +0x14 and +0x3C. getGroupNumeralString indexes them with a 0..9 range check and
// falls back to the first. postProcessLoad 0x000904CA fills them (a string per numeral, one
// for the formation letter). Size 0x40.
#pragma once
#ifndef BFME2_W3DDISPLAYSTRINGMANAGER_H
#define BFME2_W3DDISPLAYSTRINGMANAGER_H

#include "GameClient/DisplayStringManager.h"

class W3DDisplayStringManager : public DisplayStringManager
{
public:
	enum { MAX_GROUPS = 10 };

	W3DDisplayStringManager();														// 0x00090360
	virtual ~W3DDisplayStringManager();												// 0x0009045E

	virtual void postProcessLoad();													// 0x000904CA (slot 3)
	virtual void reset();															// 0x00090430
	virtual void update();															// 0x000903B0
	virtual DisplayString *newDisplayString();										// 0x000905F0
	virtual void freeDisplayString(DisplayString *string);							// 0x00090381
	virtual DisplayString *getGroupNumeralString(int numeral);						// 0x00090417
	virtual DisplayString *getFormationLetterDisplayString() { return m_formationLetterDisplayString; }	// 0x005C4AF1

protected:
	DisplayString *m_groupNumeralStrings[MAX_GROUPS];								// +0x14
	DisplayString *m_formationLetterDisplayString;									// +0x3C
};
// SIZE 0x40

#endif
