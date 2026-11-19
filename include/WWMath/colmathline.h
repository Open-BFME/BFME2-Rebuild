// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// colmathline.h: WWMath's inline line-segment overlap test (Zero Hour's colmathline.h), included only by the .cpp files that use it.
// Target facts: RayCollisionTestClass::Cull 0x00197DE0 expands it (centre = (min + max) * 0.5, extent = (max - min) * 0.5 into a stack
// box, then the call to Overlap_Test(AABox, LineSeg) 0x00723870); the box initialisation is Zero Hour's AABoxClass::Init_Min_Max.
#pragma once
#ifndef BFME2_COLMATHLINE_H
#define BFME2_COLMATHLINE_H

#include "WWMath/CollisionMath.h"
#include "WWMath/AABoxClass.h"

inline CollisionMath::OverlapType CollisionMath::Overlap_Test(const Vector3 &min, const Vector3 &max, const LineSegClass &line)
{
	AABoxClass box;
	box.Center = (max + min) * 0.5f;
	box.Extent = (max - min) * 0.5f;
	return CollisionMath::Overlap_Test(box, line);
}

#endif
