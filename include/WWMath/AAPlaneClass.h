// AAPlaneClass.h: WWMath's axis-aligned plane, the coordinate axis it faces and its distance along it.
// Target facts: CollisionMath::Collide(const LineSegClass &, const AAPlaneClass &, ...) 0x00713FE0 reads the
// axis index at +0x00 (it indexes the segment's start point and difference) and the distance at +0x04. SIZE 0x08.
#pragma once
#ifndef BFME2_AAPLANECLASS_H
#define BFME2_AAPLANECLASS_H

#include "Common/GameType.h"

class AAPlaneClass
{
public:
	enum AxisType
	{
		XNORMAL = 0,
		YNORMAL = 1,
		ZNORMAL = 2
	};

	AAPlaneClass(void) {}
	AAPlaneClass(AxisType normal, Real dist) : Normal(normal), Dist(dist) {}

	int Normal;		// +0x00: axis index
	Real Dist;		// +0x04
};
typedef char AAPlaneClassSize[(sizeof(AAPlaneClass) == 0x08) ? 1 : -1];

#endif
