// BfmeTargetCW: the callback target the parser registration forwards its two pointers to.
// Target facts: slot 1 is the forwarded question (retail bfmeAskCW 0x00306EDC calls
// [vtable+4] with the two pointers, first then second). Slot 0 is not recovered.
// PROVISIONAL: the slot names are role names; the callers only need the layout.
#pragma once
#ifndef BFME2_COMMON_BFMETARGETCW_H
#define BFME2_COMMON_BFMETARGETCW_H

#include "Common/GameType.h"

class BfmeTargetCW
{
public:
	virtual void bfmeSpare000CW(void) = 0;						// slot 0, not recovered
	virtual UnsignedByte bfmeDoCW(void *first, void *second) = 0;	// slot 1
};

#endif
