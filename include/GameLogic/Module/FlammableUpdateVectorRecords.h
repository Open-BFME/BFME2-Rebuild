// FlammableUpdateVectorRecords.h: the opaque eight-byte elements whose vector
// operations retail keeps out of line in the FlammableUpdate unit. Their
// lifetime hooks are the rowed FireFX placement copy (0x0048C81F) and range
// destructor (0x0048CE25), so no application identity is claimed for them.
#pragma once
#ifndef BFME2_FLAMMABLEUPDATEVECTORRECORDS_H
#define BFME2_FLAMMABLEUPDATEVECTORRECORDS_H

#include "Common/Vector.h"

// The element of the fill operation at 0x0048C872.
struct Rva0048C872Element
{
	unsigned words[2];
};
// SIZE 0x8

// The element of the overflow operation at 0x0048D48F.
struct Rva0048D48FRecord
{
	Rva0048D48FRecord();
	Rva0048D48FRecord(const Rva0048D48FRecord &that);
	~Rva0048D48FRecord();
	Rva0048D48FRecord &operator=(const Rva0048D48FRecord &that);

private:
	char m_bytes[8];
};
// SIZE 0x8

// The element of the push_back at 0x0048D628.
struct Rva0048D628Record
{
	Rva0048D628Record();
	Rva0048D628Record(const Rva0048D628Record &that);
	~Rva0048D628Record();
	Rva0048D628Record &operator=(const Rva0048D628Record &that);

private:
	char m_bytes[8];
};
// SIZE 0x8

// The instance the 0x0048D65F parse proc appends to: its fire-effect vector
// sits at +0x28.
struct Rva0048D65FOwner
{
	unsigned char m_pad[0x28];					// 0x00
	_STL::vector<Rva0048D628Record> m_fx;		// 0x28
};

#endif
