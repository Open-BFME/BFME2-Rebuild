// CastResultStruct.h: the result record of WWMath's ray and sweep tests.
// Target facts: the line against plane test 0x00714060 stores the fraction at +0x04, the surface normal at
// +0x08..+0x10 and, when the flag at +0x18 is set, the contact point (start + difference * fraction) at
// +0x1C..+0x24. The first byte is the "started inside" flag and +0x14 the surface type word. SIZE 0x28.
#pragma once
#ifndef BFME2_CASTRESULTSTRUCT_H
#define BFME2_CASTRESULTSTRUCT_H

#include "WWMath/Vector3.h"

struct CastResultStruct
{
	CastResultStruct(void) : StartBad(false), Fraction(1.0f), Normal(0.0f, 0.0f, 0.0f), SurfaceType(0), ComputeContactPoint(false), ContactPoint(0.0f, 0.0f, 0.0f) {}

	bool StartBad;			// +0x00
	Real Fraction;			// +0x04
	Vector3 Normal;			// +0x08
	unsigned long SurfaceType;	// +0x14
	bool ComputeContactPoint;	// +0x18
	Vector3 ContactPoint;		// +0x1C
};
typedef char CastResultStructSize[(sizeof(CastResultStruct) == 0x28) ? 1 : -1];

#endif
