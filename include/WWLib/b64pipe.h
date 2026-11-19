// b64pipe.h: WWLib's Base64 push filter (WWVegas/WWLib/b64pipe.h).
// Target facts (retail game.dat): Base64Pipe derives from Pipe (0xC) and adds the
// direction at +0xC, the pending byte count at +0x10, the 4-byte encoded buffer at
// +0x14 and the 3-byte plain buffer at +0x18 (SIZE 0x1C). Put is 0x0061B170 and
// Flush 0x0061B2F0. Base64_Encode 0x0061EAB0 and Base64_Decode 0x0061EBD0 take
// (source, length, destination, destination size).
// Donor facts: names follow Zero Hour's WWLib/b64pipe.h (GPL), naming only.
#pragma once
#ifndef BFME2_B64PIPE_H
#define BFME2_B64PIPE_H

#include "WWLib/pipe.h"

int Base64_Encode(const void *source, int slen, void *dest, int dlen);
int Base64_Decode(const void *source, int slen, void *dest, int dlen);

class Base64Pipe : public Pipe
{
public:
	typedef enum CodeControl {
		ENCODE,
		DECODE
	} CodeControl;

	Base64Pipe(CodeControl control) : Control(control), Counter(0) {}

	virtual int Flush();					// 0x0061B2F0
	virtual int Put(const void *source, int slen);	// 0x0061B170

private:
	CodeControl Control;	// +0x0C
	int Counter;		// +0x10
	char CBuffer[4];	// +0x14
	char PBuffer[3];	// +0x18

	Base64Pipe(Base64Pipe &rvalue);
	Base64Pipe &operator=(const Base64Pipe &pipe);
};
// SIZE 0x1C

#endif
