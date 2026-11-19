// FloatingTextData: one rising text entry of InGameUI's floating-text list.
// Retail evidence: the constructor 0x0029D352 stores the table VA 0x00BFD1C0,
// zeroes the colour (+4), the frame counter (+0x20), the timeout frame (+0x1C)
// and the position (+0x10..+0x18), clears the text (+8) and takes a display
// string from TheDisplayStringManager (+0xC); the destructor 0x0029D3B0 frees
// that display string. InGameUI::addFloatingText 0x002A0D19 allocates 0x24
// bytes. The only virtual is the destructor (deleting form 0x0029F762).
#pragma once
#ifndef BFME2_FLOATINGTEXTDATA_H
#define BFME2_FLOATINGTEXTDATA_H

#include "Common/UnicodeString.h"
#include "Common/Coord3D.h"

class DisplayString;

class FloatingTextData
{
public:
	FloatingTextData();							// 0x0029D352
	virtual ~FloatingTextData();				// 0x0029D3B0

	int m_color;								// +0x04
	UnicodeString m_text;						// +0x08
	DisplayString *m_dString;					// +0x0C
	Coord3D m_pos3D;							// +0x10
	unsigned int m_frameTimeOut;				// +0x1C, client frame after which it fades
	int m_frameCount;							// +0x20, frames it has been shown (signed: drawFloatingText 0x0029D200 converts it with cvtsi2ss)
};
// SIZE 0x24

#endif
