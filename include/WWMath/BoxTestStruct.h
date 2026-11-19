// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// BoxTestStruct.h: the record colmath.cpp's line against box test works on (Zero Hour's colmathline.cpp keeps it
// local to the unit; retail's Test_Aligned_Box 0x00714300 mangles it as a global struct). The box corners, the
// ray, and the result: fraction along the ray, whether it started inside, and the axis and side that were hit.
// Retail's CollisionMath::Collide(LineSegClass, AABoxClass) 0x007146E0 keeps it in a 0x40-byte frame.
#pragma once
#ifndef BFME2_BOXTESTSTRUCT_H
#define BFME2_BOXTESTSTRUCT_H

#include "WWMath/Vector3.h"

struct BoxTestStruct
{
	Vector3 Min;		// +0x00
	Vector3 Max;		// +0x0C
	Vector3 P0;		// +0x18
	Vector3 DP;		// +0x24
	float Fraction;		// +0x30
	bool Inside;		// +0x34
	int Axis;		// +0x38
	int Side;		// +0x3C
};
typedef char BoxTestStructSize[(sizeof(BoxTestStruct) == 0x40) ? 1 : -1];

#endif
