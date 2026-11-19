// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ObbCollisionStruct.h: the working state of WWMath's oriented-box sweep test (Zero Hour's colmathobbobb.cpp).
// Target facts (retail game.dat): CollisionMath::Collide(OBBoxClass, Vector3, OBBoxClass, Vector3, CastResultStruct *)
// 0x0072B5A0 builds one of these in a 0xB4-byte frame through an out-of-line constructor (0x00728810, ecx = the
// struct, four pointer arguments) and hands it to collide_obb_obb 0x0072A5C0. SIZE 0xB4.
#pragma once
#ifndef BFME2_OBBCOLLISIONSTRUCT_H
#define BFME2_OBBCOLLISIONSTRUCT_H

#include "WWMath/OBBoxClass.h"
#include "WWMath/Vector3.h"

struct CastResultStruct;

struct ObbCollisionStruct
{
	ObbCollisionStruct(const OBBoxClass &box0, const Vector3 &move0, const OBBoxClass &box1, const Vector3 &move1);	// 0x00728810

	bool StartBad;			// Inital configuration is intersecting?
	float MaxFrac;			// Longest move allowed so far
	int AxisId;			// Last separating axis
	int Side;			// which side of the interval

	int TestAxisId;			// Axis 'id' we're working on
	Vector3 TestAxis;		// Axis that we're working on

	Vector3 C;			// Vector from the center0 to center1
	Vector3 M;			// Move vector relative to stationary box0

	Vector3 A[3];			// basis vectors for box0
	Vector3 B[3];			// basis vectors for box1
	float AB[3][3];			// dot products of the basis vectors

	const OBBoxClass &Box0;
	const Vector3 &Move0;
	const OBBoxClass &Box1;
	const Vector3 &Move1;

private:
	//not implemented
	ObbCollisionStruct(const ObbCollisionStruct &);
	ObbCollisionStruct &operator=(const ObbCollisionStruct &);
};

typedef char ObbCollisionStructSize[(sizeof(ObbCollisionStruct) == 0xB4) ? 1 : -1];

bool collide_obb_obb(ObbCollisionStruct &context, CastResultStruct *result);	// 0x0072A5C0

#endif
