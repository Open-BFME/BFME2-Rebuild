// FrustumClass.h: WWMath's view frustum, the camera transform, its six
// bounding planes, its eight corners and their axis-aligned bounds.
// Target facts: the comdat default constructor 0x000F0F5B runs the vector
// constructor iterator over three 0x10-byte rows at +0x00 (the Matrix3D),
// six PlaneClass at +0x30 (PlaneClass() 0x000F0E6C) and eight Vector3 at
// +0x90; CollisionMath::Overlap_Test(const FrustumClass &, const Vector3 &)
// 0x00719940 walks the planes from +0x30; GridCullSystemClass::init_volume(
// const FrustumClass &) 0x0071A840 reads the bounds at +0xF0 and +0xFC.
// SIZE 0x108.
#pragma once
#ifndef BFME2_FRUSTUMCLASS_H
#define BFME2_FRUSTUMCLASS_H

#include "WWMath/Matrix3D.h"
#include "WWMath/PlaneClass.h"
#include "WWMath/Vector3.h"

class FrustumClass
{
public:
	const Vector3 &Get_Bound_Min(void) const { return BoundMin; }
	const Vector3 &Get_Bound_Max(void) const { return BoundMax; }

	Matrix3D CameraTransform;	// +0x00
	PlaneClass Planes[6];		// +0x30
	Vector3 Corners[8];		// +0x90
	Vector3 BoundMin;		// +0xF0
	Vector3 BoundMax;		// +0xFC
};

typedef char FrustumClassSize[(sizeof(FrustumClass) == 0x108) ? 1 : -1];

#endif
