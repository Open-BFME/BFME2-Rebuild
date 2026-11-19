// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Quaternion.h: WWMath's rotation quaternion and its interpolation helpers.
// Target facts (quat.cpp, O007173E0): Quaternion(const Vector3 &axis, float
// angle) 0x007173E0 stores sin(angle/2) times the axis at +0..+8 and
// cos(angle/2) at +0xC; Normalize 0x00717460 scales all four floats by
// WWMath::Inv_Sqrt of their squared length unless it is zero; Make_Closest
// 0x007174E0 negates all four when the dot product with the target is
// negative. SIZE 0x10.
#pragma once
#ifndef BFME2_QUATERNION_H
#define BFME2_QUATERNION_H

#include "WWMath/Vector3.h"

class Matrix3;
class Matrix3D;
class Matrix4;

class Quaternion
{
public:
	Quaternion(void) {}
	Quaternion(float a, float b, float c, float d) { X = a; Y = b; Z = c; W = d; }
	Quaternion(const Vector3 &axis, float angle);					// 0x007173E0
	// Memberwise like Vector3's: HTreeClass::Control_Bone 0x00162060 copies a Build_Quaternion result into a capture
	// record through the returned pointer, four loads and four stores, without forming the destination address first.
	__forceinline Quaternion &operator=(const Quaternion &q) { X = q.X; Y = q.Y; Z = q.Z; W = q.W; return *this; }

	float &operator[](int i) { return (&X)[i]; }
	const float &operator[](int i) const { return (&X)[i]; }

	void Make_Identity(void) { Set(0.0f, 0.0f, 0.0f, 1.0f); }
	void Set(float a, float b, float c, float d) { X = a; Y = b; Z = c; W = d; }

	void Normalize(void);								// 0x00717460
	// Zero Hour's Rotate_Vector / Post_Multiply (retail expands both in HTreeClass::Base_Update 0x001628A0 and Capture_Update 0x001622B0).
	void Rotate_Vector(const Vector3 &v, Vector3 *result) const
	{
		float x = W * v.X + (Y * v.Z - v.Y * Z);
		float y = W * v.Y - (X * v.Z - v.X * Z);
		float z = W * v.Z + (X * v.Y - v.X * Y);
		float w = -(X * v.X + Y * v.Y + Z * v.Z);

		result->X = W * x - w * X + (Y * z - y * Z);
		result->Y = W * y - w * Y - (X * z - x * Z);
		result->Z = W * z - w * Z + (X * y - Y * x);
	}
	// evidence: retail 0x001622B0 (HTreeClass::Capture_Update) expands the quaternion post-multiply at both of its PostMul sites (no call in the 1335-byte body).
	__forceinline void PostMultiply(const Quaternion &q)
	{
		Quaternion a = *this;
		X = a.W * q.X + a.X * q.W + (a.Y * q.Z - a.Z * q.Y);
		Y = a.W * q.Y + a.Y * q.W - (a.X * q.Z - a.Z * q.X);
		Z = a.W * q.Z + a.Z * q.W + (a.X * q.Y - a.Y * q.X);
		W = a.W * q.W - (a.X * q.X + a.Y * q.Y + a.Z * q.Z);
	}
	Quaternion &Make_Closest(const Quaternion &qto);				// 0x007174E0
	void Rotate_X(float theta);							// 0x00718120
	void Rotate_Y(float theta);							// 0x00718230
	void Rotate_Z(float theta);							// 0x00718340
	void Randomize(void);								// 0x007184E0

	float X;	// +0x0
	float Y;	// +0x4
	float Z;	// +0x8
	float W;	// +0xC
};

// Multiply two quaternions (Zero Hour's quat.h)
// evidence: retail 0x00718120, 0x00718230 and 0x00718340 (Quaternion::Rotate_X/Y/Z) expand this product in place.
__forceinline Quaternion operator*(const Quaternion &a, const Quaternion &b)
{
	return Quaternion
	(
		a.W * b.X + b.W * a.X + (a.Y * b.Z - b.Y * a.Z),
		a.W * b.Y + b.W * a.Y - (a.X * b.Z - b.X * a.Z),
		a.W * b.Z + b.W * a.Z + (a.X * b.Y - b.X * a.Y),
		a.W * b.W - (a.X * b.X + a.Y * b.Y + a.Z * b.Z)
	);
}

typedef char QuaternionSize[(sizeof(Quaternion) == 0x10) ? 1 : -1];

// Precomputed terms for repeated interpolation between the same two
// orientations (Slerp_Setup fills it, Cached_Slerp uses it).
struct SlerpInfoStruct
{
	float SinT;
	float Theta;
	bool Flip;
	bool Linear;
};

void Slerp(Quaternion &result, const Quaternion &a, const Quaternion &b, float t);		// 0x007176D0
void BFME2_Nlerp(Quaternion &result, const Quaternion &a, const Quaternion &b, float t);	// 0x00717550
void Slerp_Setup(const Quaternion &p, const Quaternion &q, SlerpInfoStruct *slerpinfo);	// 0x00717850
Quaternion Cached_Slerp(const Quaternion &p, const Quaternion &q, float alpha, SlerpInfoStruct *slerpinfo);	// 0x00717910
void Cached_Slerp(const Quaternion &p, const Quaternion &q, float alpha, SlerpInfoStruct *slerpinfo, Quaternion *set_q);	// 0x00717A40
Quaternion Build_Quaternion(const Matrix3D &mat);					// 0x00717B40
Quaternion Build_Quaternion(const Matrix3 &mat);					// 0x00717CB0
Quaternion Build_Quaternion(const Matrix4 &mat);					// 0x00717E30
Matrix4 Build_Matrix4(const Quaternion &quat);						// 0x00717FA0

#endif
