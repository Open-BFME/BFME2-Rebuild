// SliderData.h: the slider gadget's user data (Zero Hour's SliderData).
// Target facts: only the members the listbox's slider update 0x003249D2 touches are established
// (it zeroes +0x0, stores the list's scroll range at +0x4 and the thumb travel per unit at +0x8).
// The record's size and the remaining members are not recovered here.
#pragma once
#ifndef BFME2_SLIDERDATA_H
#define BFME2_SLIDERDATA_H

#include "Common/GameType.h"

struct _SliderData
{
	Int m_minVal;			// +0x0
	Int m_maxVal;			// +0x4
	Real m_numTicks;		// +0x8
};

#endif
