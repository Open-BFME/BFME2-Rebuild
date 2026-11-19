// b64straw.h: WWLib's Base64 pull filter (WWVegas/WWLib/b64straw.h).
// Target facts (retail game.dat): Base64Straw derives from Straw (0xC) and has the
// same four members as Base64Pipe at the same offsets: direction +0xC, pending count
// +0x10, encoded buffer +0x14 (4 bytes), plain buffer +0x18 (3 bytes); SIZE 0x1C.
// Get is 0x0061B350 (slot 2). Donor facts: names follow Zero Hour's WWLib/b64straw.h.
#pragma once
#ifndef BFME2_B64STRAW_H
#define BFME2_B64STRAW_H

#include "WWLib/straw.h"
#include "WWLib/b64pipe.h"

class Base64Straw : public Straw
{
public:
	typedef enum CodeControl {
		ENCODE,
		DECODE
	} CodeControl;

	Base64Straw(CodeControl control) : Control(control), Counter(0) {}

	virtual int Get(void *source, int slen);		// 0x0061B350

private:
	CodeControl Control;	// +0x0C
	int Counter;		// +0x10
	char CBuffer[4];	// +0x14
	char PBuffer[3];	// +0x18

	Base64Straw(Base64Straw &rvalue);
	Base64Straw &operator=(const Base64Straw &pipe);
};
// SIZE 0x1C

#endif
