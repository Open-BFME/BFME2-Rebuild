// crc.h: WWLib's rolling checksum engine and the table CRC-32 helpers.
// CRCEngine SIZE 0x0C: running value +0, staging index +4, a four-byte staging buffer +8
// that is also read as one long. Target facts (retail game.dat): Buffer_Needs_Data 0x006199B0
// is `Index != 0`; Value 0x006199C0 is the running value, or, when the staging buffer holds
// bytes, that value rotated left by one plus the staging long; operator()(char) 0x006199D0
// stores a byte and, at four, folds the staging long in the same way and clears it; the buffer
// form 0x00619A00 drains a partly filled staging buffer, folds whole longs, then stages the tail.
// CRC::Memory 0x00619AC0 and CRC::String 0x00619B00 are the table CRC-32 (start and end
// complemented) over the 256-entry table at VA 0x00DD5F38, the reflected polynomial 0xEDB88320.
// crc.cpp owns the four out-of-line bodies; the other two are emitted from this header.
#pragma once
#ifndef BFME2_CRC_H
#define BFME2_CRC_H

#include <stdlib.h>

// The case-insensitive string CRC used by the chained hash table.
unsigned long CRC_Stringi(const char *string, unsigned long crc);

class CRCEngine
{
public:
	CRCEngine(long initial = 0) : CRC(initial), Index(0) { StagingBuffer.Composite = 0; }

	void operator()(char datum);				// 0x006199D0
	long operator()(const void *buffer, int length);	// 0x00619A00
	operator long() const { return Value(); }

protected:
	bool Buffer_Needs_Data() const { return Index != 0; }	// 0x006199B0
	long Value() const						// 0x006199C0
	{
		long crc = CRC;
		if (Buffer_Needs_Data()) crc = _lrotl(crc, 1) + StagingBuffer.Composite;
		return crc;
	}

	long CRC;						// +0x00
	int Index;						// +0x04
	union {
		long Composite;
		char Buffer[sizeof(long)];
	} StagingBuffer;					// +0x08
};
// SIZE 0x0C

class CRC
{
public:
	static unsigned long Memory(unsigned char *data, unsigned long length, unsigned long crc = 0);	// 0x00619AC0
	static unsigned long String(const char *string, unsigned long crc = 0);				// 0x00619B00
};

#endif
