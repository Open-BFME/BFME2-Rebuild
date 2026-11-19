// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_TRIANGLESIDE_H
#define BFME2_TRIANGLESIDE_H

#include "WWMath/Vector3.h"

// A mesh edge whose endpoints are ordered by the hashes of their coordinate bits.
struct TriangleSide
{
	TriangleSide(const Vector3 &l1, const Vector3 &l2);
	bool operator==(const TriangleSide &s);

	Vector3 loc1;
	Vector3 loc2;
};

#endif
