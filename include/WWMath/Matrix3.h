// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Matrix3.h: WWMath's 3x3 rotation matrix (WWVegas/WWMath/matrix3.h): three row vectors.
// Target facts (retail game.dat): SIZE 0x24, rows of three floats (Vector3), row-major; Matrix3D::Set_Rotation
// 0x00711EF0 copies the nine floats into the left 3x3 block of a Matrix3D (skipping its translation column)
// and Matrix3D::Set 0x00711E70 additionally stores the position vector into that column. The map spells the
// class `Matrix3` in these rows and `Matrix3x3` (WWMath/Matrix3x3.h) in matrix3.cpp's.
#pragma once
#ifndef BFME2_MATRIX3_H
#define BFME2_MATRIX3_H

#include "WWMath/Vector3.h"

class Matrix3D;

class Matrix3
{
public:
	Matrix3() {}
	// Zero Hour's explicit identity constructor (OBBoxClass(center, extent) 0x000B29D3 builds its basis with it).
	__forceinline explicit Matrix3(bool identity)
	{
		if (identity) {
			Row[0].Set(1.0, 0.0, 0.0);
			Row[1].Set(0.0, 1.0, 0.0);
			Row[2].Set(0.0, 0.0, 1.0);
		}
	}
	// Zero Hour's constructor from three rows (Transpose builds its result through it).
	__forceinline Matrix3(const Vector3 &r0, const Vector3 &r1, const Vector3 &r2)
	{
		Row[0] = r0;
		Row[1] = r1;
		Row[2] = r2;
	}

	Matrix3 Transpose(void) const;								// 0x00713ED0
	void Rotate_AABox_Extent(const Vector3 &extent, Vector3 *set_extent);			// 0x00149310 (pushes an axis-aligned box's extent through the absolute-value matrix)
	void Set(const Matrix3D &m);								// 0x007168F0 (the rotation part of a transform: OBBoxRenderObjClass::update_cached_box 0x001758C0)

	// Zero Hour's out-of-line Multiply(Matrix3D, Matrix3x3): OBBoxClass::Transform 0x000B2A4C calls it.
	static void Multiply(const Matrix3D &a, const Matrix3 &b, Matrix3 *res);			// 0x00716A20

	// WWMath's point rotations; the output may alias the input (retail copies the input first then).
	static void Rotate_Vector(const Matrix3 &A, const Vector3 &in, Vector3 *out);			// 0x00714C00
	static void Transpose_Rotate_Vector(const Matrix3 &A, const Vector3 &in, Vector3 *out);	// 0x00714CC0

	__forceinline Vector3 &operator[](int i) { return Row[i]; }
	__forceinline const Vector3 &operator[](int i) const { return Row[i]; }

private:
	Vector3 Row[3];		// +0x00
};
// SIZE 0x24

// Matrix times column vector (0x00713F50).
Vector3 operator*(const Matrix3 &A, const Vector3 &v);

#endif
