// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// TriClass.h: a triangle that points at its normal and its three corners (Zero Hour's tri.h; retail's
// TriClass::Contains_Point is 0x00724360 and Find_Dominant_Plane 0x00724290). SIZE 0x10.
#pragma once
#ifndef BFME2_TRICLASS_H
#define BFME2_TRICLASS_H

#include "WWMath/Vector3.h"

class TriClass
{
public:
	const Vector3 *N;		// +0x00
	const Vector3 *V[3];		// +0x04

	// Zero Hour's tri.h text (plain inline: retail's unit BaseHeightMap.cpp emits an out-of-line copy, 0x0006A0CA, while the
	// polygon loops that fill a triangle's normal in place expand it; h03's request, evidence under that request).
	void Compute_Normal()
	{
		Vector3::Cross_Product(*(V[1]) - *(V[0]), *(V[2]) - *(V[0]), (Vector3 *)N);
		((Vector3 *)N)->Normalize();
	}

	bool Contains_Point(const Vector3 &ipoint) const;			// 0x00724360
	void Find_Dominant_Plane(int *axis1, int *axis2) const;			// 0x00724290
};
typedef char TriClassSize[(sizeof(TriClass) == 0x10) ? 1 : -1];

#endif
