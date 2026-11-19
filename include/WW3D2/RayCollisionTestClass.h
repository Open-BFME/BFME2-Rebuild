// RayCollisionTestClass.h: the input of a ray cast through the render objects (RenderObjClass::Cast_Ray, slot 60).
// Layout from Zero Hour's coltest.h (CollisionTestClass: result, type, hit object; then the segment and two
// flags), checked against retail's reader HLodClass::Cast_Ray 0x0019CF50: the byte at +0x42 follows the two
// flags Zero Hour has at +0x40 and +0x41 and makes the cast stop at the first hit.
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_RAYCOLLISIONTESTCLASS_H
#define BFME2_RAYCOLLISIONTESTCLASS_H

#include "WWMath/Vector3.h"
#include "WWMath/LineSegClass.h"
#include "WWMath/CastResultStruct.h"
#include "WW3D2/coltype.h"

class RenderObjClass;

class CollisionTestClass
{
public:
	CollisionTestClass(CastResultStruct *res, int collision_type) : Result(res), CollisionType(collision_type), CollidedRenderObj(0) {}

	CastResultStruct *Result;		// +0x00
	int CollisionType;			// +0x04
	RenderObjClass *CollidedRenderObj;	// +0x08
};

class RayCollisionTestClass : public CollisionTestClass
{
public:
	bool Cull(const Vector3 &min, const Vector3 &max);		// 0x00197DE0 (Zero Hour's inline; retail's copy is a row of aabtree.cpp)

	RayCollisionTestClass(const LineSegClass &ray, CastResultStruct *res, int collision_type = COLL_TYPE_0, bool check_translucent = false, bool check_hidden = false)
		: CollisionTestClass(res, collision_type), Ray(ray), CheckTranslucent(check_translucent), CheckHidden(check_hidden), StopAtFirstHit(false) {}

	LineSegClass Ray;		// +0x0C
	bool CheckTranslucent;		// +0x40
	bool CheckHidden;		// +0x41
	bool StopAtFirstHit;		// +0x42
};


#endif
