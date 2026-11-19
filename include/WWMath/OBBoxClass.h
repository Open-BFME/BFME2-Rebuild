// OBBoxClass.h: WWMath's oriented box, a rotation basis, a centre and a
// half-size extent along the basis axes.
// Target facts: Compute_Axis_Aligned_Extent 0x00719D10 multiplies the nine
// basis floats at +0x00..+0x20 by the extent at +0x30..+0x38 and writes the
// summed absolute values; GridCullSystemClass::init_volume(const OBBoxClass &)
// 0x0071A560 calls it out of line and reads the centre at +0x24..+0x2C.
// SIZE 0x3C (the copy assignment 0x00133990 moves fifteen dwords).
#pragma once
#ifndef BFME2_OBBOXCLASS_H
#define BFME2_OBBOXCLASS_H

#include "WWMath/Matrix3.h"
#include "WWMath/Matrix3D.h"
#include "WWMath/Vector3.h"

class OBBoxClass
{
public:
	OBBoxClass() {}
	OBBoxClass(const Vector3 &center, const Vector3 &extent);		// 0x000B29D3
	static void Transform(const Matrix3D &tm, const OBBoxClass &in, OBBoxClass *out);	// 0x000B2A4C
	inline void Compute_Axis_Aligned_Extent(Vector3 *set_extent) const;	// 0x00719D10

	// Matrix3 (WWMath's 3x3): Collide(LineSegClass, OBBoxClass) 0x00714860 calls Matrix3::Transpose 0x00713ED0 on it.
	Matrix3 Basis;		// +0x00
	Vector3 Center;		// +0x24
	Vector3 Extent;		// +0x30
};

typedef char OBBoxClassSize[(sizeof(OBBoxClass) == 0x3C) ? 1 : -1];

// The half-size of the axis-aligned box around this one: per world axis, the
// summed absolute projections of the three extent vectors. A plain inline that
// gridcull.cpp's unit does not expand (its callers 0x0071A560 and 0x0071BBC0
// call the comdat), yet its body being visible lets those callers reuse the
// result vector's stack slot.
inline void OBBoxClass::Compute_Axis_Aligned_Extent(Vector3 *set_extent) const
{
	// Row r of the basis gives how much each local extent leans along world axis r.
	set_extent->X = WWMath::Fabs(Extent[0] * Basis[0][0]) + WWMath::Fabs(Extent[1] * Basis[0][1]) + WWMath::Fabs(Extent[2] * Basis[0][2]);
	set_extent->Y = WWMath::Fabs(Extent[0] * Basis[1][0]) + WWMath::Fabs(Extent[1] * Basis[1][1]) + WWMath::Fabs(Extent[2] * Basis[1][2]);
	set_extent->Z = WWMath::Fabs(Extent[0] * Basis[2][0]) + WWMath::Fabs(Extent[1] * Basis[2][1]) + WWMath::Fabs(Extent[2] * Basis[2][2]);
}

#endif
