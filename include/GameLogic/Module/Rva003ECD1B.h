// Rva003ECD1B.h: the small update body at 0x003ECD1B that copies a three-float
// position from an owner record into a child record when the child exists.
// Target facts (old repo ThreatFinderUpdate.cpp): the receiver is an interior
// view at root+8, the owner record holds its position at +0x38, the child at
// +0x554, and the body returns 1. The enclosing class stays unnamed.
#pragma once
#ifndef BFME2_RVA003ECD1B_H
#define BFME2_RVA003ECD1B_H

struct Rva003ECD1BVector
{
	Rva003ECD1BVector(const Rva003ECD1BVector &that)
	{
		x = that.x;
		y = that.y;
		z = that.z;
	}
	float x;
	float y;
	float z;
};

struct Rva003ECD1BObject
{
	char m_pad00[0x38];
	Rva003ECD1BVector m_position;		// 0x38
};

struct Rva003ECD1BChild
{
	char m_pad00[0x554];
	Rva003ECD1BVector m_position;		// 0x554
};

struct Rva003ECD1BRoot
{
	Rva003ECD1BObject *m_object;		// 0x00
	char m_pad04[0x14];
	Rva003ECD1BChild *m_child;			// 0x18
};

class Rva003ECD1B
{
public:
	int update();
};

#endif
