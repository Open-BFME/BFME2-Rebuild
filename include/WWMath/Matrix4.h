// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Matrix4.h: WWMath's 4x4 matrix (matrix4.h).
// Target facts: four Vector4 rows, 0x40 bytes (operator*(Matrix4, Vector3)
// 0x000A638D reads +0x00..+0x3C as Row[i][j]). Out of line in WWMath/matrix4.cpp:
// Multiply x3 (0x00715550, 0x007158F0, 0x00715BC0), Get_Inverse 0x00715E90,
// Inverse 0x00715EB0. Init(Matrix3D) 0x00068FEF, Transpose 0x0006A3D1,
// Make_Identity 0x00076735, Init_Ortho 0x00133510 and Init_Perspective
// 0x00133610 exist only as uncalled copies of header inlines; they are declared
// until their bodies are needed. operator*(Matrix4, Vector3) is emitted in
// W3DSmudge.cpp's object (0x000A638D) and defined there.
#pragma once
#ifndef BFME2_MATRIX4_H
#define BFME2_MATRIX4_H

#include "WWMath/Vector3.h"
#include "WWMath/Vector4.h"
#include "WWMath/Matrix3D.h"

class Matrix3D;

class Matrix4
{
public:
	__forceinline Matrix4() {}
	// Row-by-row copy, expanded in place by its callers
	// (CameraClass::Set_Reflected_Projection_Matrix 0x00133DA0).
	__forceinline Matrix4 &operator=(const Matrix4 &m)
	{
		Row[0] = m.Row[0];
		Row[1] = m.Row[1];
		Row[2] = m.Row[2];
		Row[3] = m.Row[3];
		return *this;
	}

	Vector4 &operator[](int i) { return Row[i]; }
	const Vector4 &operator[](int i) const { return Row[i]; }

	// Zero Hour's WWINLINE conversion from a Matrix3D (a fake 4x4): Matrix3D::Get_Inverse 0x00713290 and the inverse at
	// 0x007133F0 expand it in place (twelve row stores, then 0, 0, 0, 1).
	__forceinline explicit Matrix4(const Matrix3D &m) { Init(m); }
	__forceinline void Init(const Matrix3D &m)
	{
		Row[0] = m[0]; Row[1] = m[1]; Row[2] = m[2]; Row[3] = Vector4(0.0, 0.0, 0.0, 1.0);
	}
	// evidence: retail DX8Wrapper::Set_World_Identity 0x0010737E / Set_View_Identity 0x001100E8 and the Scale/Grid
	// Calculate_Texture_Matrix rows expand it as twelve stores in place; 0x00076735 is the out-of-line copy.
	__forceinline void Make_Identity()
	{
		Row[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
		Row[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
		Row[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
		Row[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
	}
	// evidence: EdgeMapperClass::Calculate_Texture_Matrix 0x00183540 passes sixteen floats to it in place (the last row's VOffset is
	// spilled and reloaded as an argument); Zero Hour's WWINLINE Init.
	__forceinline void Init(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
	{
		Row[0].Set(m11, m12, m13, m14);
		Row[1].Set(m21, m22, m23, m24);
		Row[2].Set(m31, m32, m33, m34);
		Row[3].Set(m41, m42, m43, m44);
	}
	// Zero Hour's four-row constructor (Transpose builds its result through it).
	__forceinline Matrix4(const Vector4 &r0, const Vector4 &r1, const Vector4 &r2, const Vector4 &r3)
	{
		Row[0] = r0; Row[1] = r1; Row[2] = r2; Row[3] = r3;
	}
	// evidence: retail 0x001844A0, 0x00184FD0, 0x00185E80 (DX8Wrapper::Set_Transform's Transpose arms) and 0x0006A3D1's callers store the transpose straight into its destination, sixteen scalar loads and stores each, with no call to 0x0006A3D1.
	__forceinline Matrix4 Transpose() const
	{
		return Matrix4(
			Vector4(Row[0][0], Row[1][0], Row[2][0], Row[3][0]),
			Vector4(Row[0][1], Row[1][1], Row[2][1], Row[3][1]),
			Vector4(Row[0][2], Row[1][2], Row[2][2], Row[3][2]),
			Vector4(Row[0][3], Row[1][3], Row[2][3], Row[3][3]));
	}
	void Init_Ortho(float left, float right, float bottom, float top, float znear, float zfar);
	void Init_Perspective(float left, float right, float bottom, float top, float znear, float zfar);
	void Get_Inverse(Matrix4 &set_inverse) const;					// 0x00715E90
	void Inverse(Matrix4 *set_inverse) const;						// 0x00715EB0

	static void Multiply(const Matrix4 &a, const Matrix4 &b, Matrix4 *res);	// 0x00715550
	static void Multiply(const Matrix3D &a, const Matrix4 &b, Matrix4 *res);	// 0x007158F0
	static void Multiply(const Matrix4 &a, const Matrix3D &b, Matrix4 *res);	// 0x00715BC0

protected:
	Vector4 Row[4];		// 0x00
};
// SIZE 0x40
typedef char Matrix4Size[(sizeof(Matrix4) == 0x40) ? 1 : -1];

Vector4 operator*(const Matrix4 &a, const Vector3 &v);			// 0x000A638D

// The same product expanded in place (w = 1): CameraClass::Project and Project_Camera_Space_Point
// 0x00135390 / 0x001355E0 expand it, while W3DSmudge.cpp owns the out-of-line copy of the operator above.
// evidence: retail expands this product at every call site: CameraClass::Project 0x00135390 and Project_Camera_Space_Point 0x001355E0 (no call), no plainer form reproduces their bytes
__forceinline Vector4 Transform_Point(const Matrix4 &a, const Vector3 &v)
{
	return Vector4(
		a[0][0] * v[0] + a[0][1] * v[1] + a[0][2] * v[2] + a[0][3] * 1.0f,
		a[1][0] * v[0] + a[1][1] * v[1] + a[1][2] * v[2] + a[1][3] * 1.0f,
		a[2][0] * v[0] + a[2][1] * v[1] + a[2][2] * v[2] + a[2][3] * 1.0f,
		a[3][0] * v[0] + a[3][1] * v[1] + a[3][2] * v[2] + a[3][3] * 1.0f);
}

// Matrix times column vector. CameraClass::Compute_Projected_Sphere_Radius
// 0x00133E80 expands it in place (rows 0 and 3 only survive there).
inline Vector4 operator*(const Matrix4 &a, const Vector4 &v)
{
	return Vector4(
		a[0][0] * v[0] + a[0][1] * v[1] + a[0][2] * v[2] + a[0][3] * v[3],
		a[1][0] * v[0] + a[1][1] * v[1] + a[1][2] * v[2] + a[1][3] * v[3],
		a[2][0] * v[0] + a[2][1] * v[1] + a[2][2] * v[2] + a[2][3] * v[3],
		a[3][0] * v[0] + a[3][1] * v[1] + a[3][2] * v[2] + a[3][3] * v[3]);
}

#endif
