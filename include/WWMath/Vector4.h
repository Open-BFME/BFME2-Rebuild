// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Vector4.h: WWMath's four-float vector (WWVegas/WWMath/vector4.h).
// Target facts: class (map rows ??0Vector4@@QAE@MMMM@Z, ??4Vector4@@QAEAAV0@ABV0@@Z,
// ?Set@Vector4@@QAEXMMMM@Z); X +0, Y +4, Z +8, W +0xC (DX8Wrapper::Convert_Color
// 0x00139290 and the colour arrays of PointGroupClass/StreakLineClass index it as
// four floats). Every member is inline, as in WWMath: the retail bodies that
// exist (the constructor COMDAT 0x006D8A30, assignment COMDAT 0x00133880, Set
// 0x0004256D, operators 0x000A60A9, 0x00181F70, 0x00191070, 0x001910C0,
// 0x00308182, 0x00711BF0, 0x007154B0, Swap 0x00715500) have no direct callers
// or are COMDAT copies, i.e. inline definitions emitted where a caller declined
// to inline them. Only the members those rows name are declared. The copy
// constructor stays implicit: retail has no row for one, and a user-declared copy
// makes Matrix3D's copy non-trivial (PhysicsBehavior::update 0x0039114B then
// grows by 8 bytes).
#pragma once
#ifndef BFME2_VECTOR4_H
#define BFME2_VECTOR4_H

#include "Common/GameType.h"
#include "WWMath/wwinline.h"

class Vector4
{
public:
	Real X;		// 0x0
	Real Y;		// 0x4
	Real Z;		// 0x8
	Real W;		// 0xC

	Vector4() {}
	WWINLINE Vector4(const Vector4 &v) { X = v.X; Y = v.Y; Z = v.Z; W = v.W; }
	Vector4(Real x, Real y, Real z, Real w) { X = x; Y = y; Z = z; W = w; }

	WWINLINE Vector4 &operator=(const Vector4 &v) { X = v.X; Y = v.Y; Z = v.Z; W = v.W; return *this; }
	void Set(Real x, Real y, Real z, Real w) { X = x; Y = y; Z = z; W = w; }

	Real &operator[](int i) { return (&X)[i]; }
	const Real &operator[](int i) const { return (&X)[i]; }

	// Zero Hour's WWINLINE operators. The fourth component is added, not subtracted, by operator-=: retail's
	// Matrix3D::Solve_Linear_System 0x00712F30 (and Zero Hour's own source) do this.
	WWINLINE Vector4 &operator+=(const Vector4 &v) { X += v.X; Y += v.Y; Z += v.Z; W += v.W; return *this; }
	WWINLINE Vector4 &operator-=(const Vector4 &v) { X -= v.X; Y -= v.Y; Z -= v.Z; W += v.W; return *this; }
	WWINLINE Vector4 &operator*=(Real k) { X = X*k; Y = Y*k; Z = Z*k; W = W*k; return *this; }
	WWINLINE Vector4 &operator/=(Real k) { k = 1.0f / k; X = X*k; Y = Y*k; Z = Z*k; W = W*k; return *this; }
};
// SIZE 0x10
typedef char Vector4Size[(sizeof(Vector4) == 0x10) ? 1 : -1];

WWINLINE Vector4 operator*(const Vector4 &a, Real k) { return Vector4((a.X * k), (a.Y * k), (a.Z * k), (a.W * k)); }
WWINLINE Vector4 operator*(Real k, const Vector4 &a) { return a * k; }
WWINLINE Vector4 operator+(const Vector4 &a, const Vector4 &b) { return Vector4(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]); }

inline void Swap(Vector4 &a, Vector4 &b)
{
	Vector4 tmp(a);
	a = b;
	b = tmp;
}

#endif
