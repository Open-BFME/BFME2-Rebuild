// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Vector2.h: WWMath's two-float vector (WWVegas/WWMath/vector2.h).
// Target facts: class (VectorProcessorClass::Copy(Vector2 *, const Vector2 *, int)
// 0x0071D3E0 mangles PAVVector2; Bfme dynamic vertex buffers take PBVVector2);
// eight bytes, X +0, Y +4 (that Copy moves count*2 dwords).
// Every member is inline as in WWMath: the retail bodies that exist (Set 0x0004CAE4
// and 0x000D43DA, Length2 0x0004238C, Dot_Product 0x000D449F, Perp_Dot_Product
// 0x0007B32D) have no direct callers, so they are copies emitted where a caller
// declined to inline. The road buffer's rotateAbout expands WWMath::Sin and Cos in place.
// The copy constructor is Zero Hour's float-wise one.
#pragma once
#ifndef BFME2_VECTOR2_H
#define BFME2_VECTOR2_H

#include "Common/GameType.h"
#include "WWMath/wwinline.h"
#include "WWMath/wwmath.h"

class Vector2
{
public:
	Real X;		// 0x0
	Real Y;		// 0x4

	Vector2() {}
	Vector2(Real x, Real y) { X = x; Y = y; }
	// Zero Hour's explicit copy constructor: RenderObjClass::Get_Screen_Size 0x0013C030 copies a ViewportClass with float moves.
	Vector2(const Vector2 &v) { X = v.X; Y = v.Y; }
	__forceinline Vector2 &operator=(const Vector2 &v) { X = v[0]; Y = v[1]; return *this; }

	WWINLINE Vector2 operator-() const { return Vector2(-X, -Y); }
	WWINLINE Vector2 &operator+=(const Vector2 &v) { X += v.X; Y += v.Y; return *this; }
	WWINLINE Vector2 &operator-=(const Vector2 &v) { X -= v.X; Y -= v.Y; return *this; }
	WWINLINE Vector2 &operator*=(Real k) { X = (Real)(X * k); Y = (Real)(Y * k); return *this; }

	// Rotation (WWMath vector2.h; RoadBuffer's rotateAbout 0x000D6911 expands both in place).
	WWINLINE void Normalize(void);
	WWINLINE void Rotate(Real theta);
	WWINLINE void Rotate(Real s, Real c);

	void Set(Real x, Real y) { X = x; Y = y; }
	void Set(const Vector2 &v) { X = v.X; Y = v.Y; }

	Real &operator[](int i) { return (&X)[i]; }
	const Real &operator[](int i) const { return (&X)[i]; }

	Real Length() const;
	Real Length2() const { return X * X + Y * Y; }

	// Operand order from the retail copies (0x000D449F loads a.Y*b.Y first).
	static Real Dot_Product(const Vector2 &a, const Vector2 &b) { return a.Y * b.Y + a.X * b.X; }
	// 0x0007B32D: a.Y*b.X - b.Y*a.X.
	static Real Perp_Dot_Product(const Vector2 &a, const Vector2 &b) { return a.Y * b.X - b.Y * a.X; }
};
// SIZE 0x8

// WWMath vector2.h (Zero Hour's text): the operator forms W3DRoadBuffer's offset3Way/offsetH/offsetY/offset4Way
// (0x000D54D1..0x000D5D10) expand in place; retail's out-of-line copies are 0x000D4451 and 0x000D4478.
WWINLINE Vector2 operator*(const Vector2 &a, Real k)
{
	Real a0k(a[0] * k);
	Real a1k(a[1] * k);
	return Vector2(a0k, a1k);
}

WWINLINE Vector2 operator*(Real k, const Vector2 &a)
{
	Real a0k(a[0] * k);
	Real a1k(a[1] * k);
	return Vector2(a0k, a1k);
}

WWINLINE Vector2 operator+(const Vector2 &a, const Vector2 &b)
{
	return Vector2(a.X + b.X, a.Y + b.Y);
}

WWINLINE Vector2 operator-(const Vector2 &a, const Vector2 &b)
{
	return Vector2(a.X - b.X, a.Y - b.Y);
}

WWINLINE void Vector2::Normalize(void)
{
	Real len2 = Length2();
	if (len2 != 0.0f) {
		Real oolen = WWMath::Inv_Sqrt(len2);
		X *= oolen;
		Y *= oolen;
	}
}

WWINLINE void Vector2::Rotate(Real theta)
{
	Rotate(WWMath::Sin(theta), WWMath::Cos(theta));
}

WWINLINE void Vector2::Rotate(Real s, Real c)
{
	Real new_x = X * c + Y * -s;
	Real new_y = X * s + Y * c;
	X = new_x;
	Y = new_y;
}
// Westwood vector2.h divides by multiplying with the reciprocal
// (SegLineRendererClass::Set_UV_Offset_Rate 0x0015E610 multiplies by 0.001f).
inline Vector2 operator/(const Vector2 &a, Real k)
{
	Real ook = 1.0f / k;
	return Vector2(a.X * ook, a.Y * ook);
}

// Zero Hour's comparison operators: both halves compare, then the two booleans combine without branching
// (retail's copies 0x000D44E5 and 0x000D451A are `and eax,ebx` and `or eax,ebx`).
WWINLINE bool operator==(const Vector2 &a, const Vector2 &b)
{
	bool x = (a.X == b.X);
	bool y = (a.Y == b.Y);
	return x & y;
}

WWINLINE bool operator!=(const Vector2 &a, const Vector2 &b)
{
	bool x = (a.X != b.X);
	bool y = (a.Y != b.Y);
	return x | y;
}

typedef char Vector2Size[(sizeof(Vector2) == 0x8) ? 1 : -1];

#endif
