// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// inttest.h: WW3D2's box intersection tests (Zero Hour's inttest.h): the request RenderObjClass::Intersect_AABox/Intersect_OBBox pass down the
// culling trees. Layout checked against retail: the OBBox test's constructor 0x0014A3F0 stores CollisionType at +0, builds the box at +4
// (basis +4, centre +0x28, extent +0x34) and calls update_bounding_box 0x001493F0, which fills the axis-aligned bounding box at +0x40
// (centre +0x40, extent +0x4C); AABTreeClass::Intersect_OBBox_Recursive 0x00198730 expands Cull in place against that bounding box. SIZE 0x58.
#pragma once
#ifndef BFME2_INTTEST_H
#define BFME2_INTTEST_H

#include "WWMath/AABoxClass.h"
#include "WWMath/OBBoxClass.h"
#include "WWMath/Vector3.h"

class Matrix3D;
class TriClass;

class IntersectionTestClass
{
public:
	IntersectionTestClass(int collision_type) : CollisionType(collision_type) {}
	IntersectionTestClass(const IntersectionTestClass &that) : CollisionType(that.CollisionType) {}

	int CollisionType;		// +0x00
};

class AABoxIntersectionTestClass : public IntersectionTestClass
{
public:
	AABoxIntersectionTestClass(const AABoxClass &box, int collision_type) : IntersectionTestClass(collision_type), Box(box) {}
	AABoxIntersectionTestClass(const AABoxIntersectionTestClass &that) : IntersectionTestClass(that), Box(that.Box) {}

	AABoxClass Box;			// +0x04 world-space box to test with
};

class OBBoxIntersectionTestClass : public IntersectionTestClass
{
public:
	OBBoxIntersectionTestClass(const OBBoxClass &box, int collision_type);
	OBBoxIntersectionTestClass(const OBBoxIntersectionTestClass &that);
	OBBoxIntersectionTestClass(const OBBoxIntersectionTestClass &that, const Matrix3D &tm);
	OBBoxIntersectionTestClass(const AABoxIntersectionTestClass &that, const Matrix3D &tm);		// 0x0014A3F0

	// Expanded in place by AABTreeClass::Intersect_OBBox_Recursive 0x00198730 (Zero Hour's inline).
	bool Cull(const Vector3 &cull_min, const Vector3 &cull_max)
	{
		Vector3 box_min;
		Vector3::Subtract(BoundingBox.Center, BoundingBox.Extent, &box_min);

		Vector3 box_max;
		Vector3::Add(BoundingBox.Center, BoundingBox.Extent, &box_max);
		if ((box_min.X > cull_max.X) || (box_max.X < cull_min.X)) return true;
		if ((box_min.Y > cull_max.Y) || (box_max.Y < cull_min.Y)) return true;
		if ((box_min.Z > cull_max.Z) || (box_max.Z < cull_min.Z)) return true;

		return false;
	}

protected:
	void update_bounding_box(void);									// 0x001493F0

public:
	OBBoxClass Box;			// +0x04 world-space oriented box to test with
	AABoxClass BoundingBox;		// +0x40 axis-aligned world-space bounding box of Box
};

typedef char OBBoxIntersectionTestClassSize[(sizeof(OBBoxIntersectionTestClass) == 0x58) ? 1 : -1];

#endif
