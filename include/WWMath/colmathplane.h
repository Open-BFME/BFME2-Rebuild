// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// colmathplane.h: WWMath's inline plane tests, included only by the .cpp files that use them.
// Target facts: CameraClass::Cull_Box 0x00135990 expands the plane/AABox test per frustum plane,
// with the sign-masked far-extent selection and the COINCIDENCE_EPSILON load (0x008EFC94).
#pragma once
#ifndef BFME2_COLMATHPLANE_H
#define BFME2_COLMATHPLANE_H

#include "WWMath/CollisionMath.h"
#include "WWMath/PlaneClass.h"
#include "WWMath/AABoxClass.h"

inline void get_far_extent(const Vector3 & normal,const Vector3 & extent,Vector3 * posfarpt)
{
	if (WWMath::Fast_Is_Float_Positive(normal.X)) {
		posfarpt->X = extent.X;
	} else {
		posfarpt->X = -extent.X;
	}

	if (WWMath::Fast_Is_Float_Positive(normal.Y)) {
		posfarpt->Y = extent.Y;
	} else {
		posfarpt->Y = -extent.Y;
	}

	if (WWMath::Fast_Is_Float_Positive(normal.Z)) {
		posfarpt->Z = extent.Z;
	} else {
		posfarpt->Z = -extent.Z;
	}
}

inline
CollisionMath::OverlapType
CollisionMath::Overlap_Test(const PlaneClass & plane,const Vector3 & point)
{
	float delta = Vector3::Dot_Product(point,plane.N) - plane.D;
	if (delta > COINCIDENCE_EPSILON) {
		return POS;
	}
	if (delta < -COINCIDENCE_EPSILON) {
		return NEG;
	}
	return ON;
}

inline
CollisionMath::OverlapType
CollisionMath::Overlap_Test(const PlaneClass & plane,const AABoxClass & box)
{
	// First, we determine the the near and far points of the box in the
	// direction of the plane normal
	Vector3 posfarpt;
	Vector3 negfarpt;

	get_far_extent(plane.N,box.Extent,&posfarpt);

	negfarpt = -posfarpt;
	posfarpt += box.Center;
	negfarpt += box.Center;
	if (Overlap_Test(plane,negfarpt) == POS) {
		return POS;
	}
	if (Overlap_Test(plane,posfarpt) == NEG) {
		return NEG;
	}
	return BOTH;
}

#endif
