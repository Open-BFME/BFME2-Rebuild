// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Matrix3D.h: WWMath 3x4 transform. SIZE 0x30.
// Target facts: Row[3][4]; Make_Identity writes 1.0f on the diagonal of the
// 3x3 block and zero elsewhere (clientUpdate inlines it as twelve stores);
// the translation is Row[i][3].
// Rows are Vector4, as WWMath's matrix3d.h has them: retail's operator[]
// (??AMatrix3D@@QAEAAVVector4@@H@Z, 0x0007DE8E) returns Vector4&.
// The constructors are __forceinline (WWMath's WWINLINE). Because Vector4 has
// user-declared constructors, constructing the row array makes a plain inline
// Matrix3D() and Matrix3D(Bool) too costly for MSVC 7.1 to inline, while
// retail inlines both: PhysicsBehavior::update 0x0039114B (empty constructor)
// and FlammableUpdate::tryToIgnite 0x0048CB18 (twelve identity stores) MATCH
// only with __forceinline.
// Out of line: the members below whose rows sit in WWMath/matrix3d.cpp
// (object O00711E70, 0x00711E70-0x00713EC9); callers reach them by call
// (e.g. Get_Z_Rotation 19 call sites, Set_Rotation(Matrix3) 9, Obj_Look_At 9,
// Get_Orthogonal_Inverse 9, Get_Inverse 7, Transform_Center_Extent_AABox 7).
#pragma once
#ifndef BFME2_MATRIX3D_H
#define BFME2_MATRIX3D_H

#include <math.h>
#include "WWMath/Vector3.h"
#include "WWMath/Vector4.h"
#include "Common/GameType.h"
class Matrix3;
class Quaternion;

class Matrix3D
{
public:
	__forceinline Matrix3D() {}
	// WWMath's initialising constructor: `true` makes it the identity.
	__forceinline Matrix3D(Bool init) { if (init) Make_Identity(); }
	// Zero Hour's copy constructor: three row copies. Retail's Thing::setPosition 0x0030AA80 copies a transform with
	// twelve movss load/store pairs instead of the `rep movsd` the implicit copy gives.
	WWINLINE Matrix3D(const Matrix3D &m)
	{
		Row[0] = m.Row[0];
		Row[1] = m.Row[1];
		Row[2] = m.Row[2];
	}
	// Zero Hour's twelve-float Set (WWINLINE; Thing::setOrientation 0x0030AB9D expands it).
	WWINLINE void Set(float m11, float m12, float m13, float m14,
					 float m21, float m22, float m23, float m24,
					 float m31, float m32, float m33, float m34)
	{
		Row[0].Set(m11, m12, m13, m14);
		Row[1].Set(m21, m22, m23, m24);
		Row[2].Set(m31, m32, m33, m34);
	}
	// Zero Hour's row copy (Vector4's memberwise assignment, three times); Matrix3D::Multiply 0x00713540 expands it.
	WWINLINE Matrix3D &operator=(const Matrix3D &m)
	{
		Row[0] = m.Row[0];
		Row[1] = m.Row[1];
		Row[2] = m.Row[2];
		return *this;
	}
    void buildTransformMatrix(const Vector3 &,const Vector3 &);		// 0x00712440

	// 0x0007DE8E is an uncalled copy of this inline (12 bytes).
	Vector4 &operator[](int i) { return Row[i]; }
	const Vector4 &operator[](int i) const { return Row[i]; }

	// WWMath/matrix3d.cpp (out of line).
	void Set(const Matrix3 &rotation, const Vector3 &position);		// 0x00711E70
	void Set_Rotation(const Matrix3 &m);								// 0x00711EF0
	void Set_Rotation(const Quaternion &q);								// 0x00711F30
	float Get_X_Rotation() const;										// 0x00712000
	float Get_Y_Rotation() const;										// 0x00712010
	float Get_Z_Rotation() const;										// 0x00712020
	Vector3 Rotate_Vector(const Vector3 &vect) const;					// 0x00712030
	Vector3 Inverse_Rotate_Vector(const Vector3 &vect) const;			// 0x007120C0
	void Look_At(const Vector3 &p, const Vector3 &t, float roll);		// 0x00712150
	void Obj_Look_At(const Vector3 &p, const Vector3 &t, float roll);	// 0x007126B0
	void Get_Orthogonal_Inverse(Matrix3D &set_inverse) const;			// 0x00712A30
	void Copy_3x3_Matrix(float matrix[3][3]);							// 0x00712B10
	void Transform_Min_Max_AABox(const Vector3 &min, const Vector3 &max, Vector3 *set_min, Vector3 *set_max) const;	// 0x00712B60
	void Transform_Center_Extent_AABox(const Vector3 &center, const Vector3 &extent, Vector3 *set_center, Vector3 *set_extent) const;	// 0x00712CA0
	int Is_Orthogonal() const;											// 0x00712D90
	static bool Solve_Linear_System(Matrix3D &system);					// 0x00712F30
	void Set(const Quaternion &rot, const Vector3 &pos);				// 0x00713260
	void Get_Inverse(Matrix3D &set_inverse) const;						// 0x00713290
	static void Multiply(const Matrix3D &a, const Matrix3D &b, Matrix3D *res);	// 0x00713540
	void Re_Orthogonalize();											// 0x00713810
	static void Lerp(const Matrix3D &a, const Matrix3D &b, float factor, Matrix3D &result);	// 0x00713AB0

	// WWMath's static point transforms; CameraClass::Transform_To_View_Space
	// 0x00135770 and Rotate_To_View_Space 0x00135870 (and LineSegClass::Set
	// 0x00718F70) expand them in place, including the copy to a temporary when
	// the output aliases the input.
	static __forceinline void Transform_Vector(const Matrix3D &tm, const Vector3 &in, Vector3 *out)
	{
		Vector3 tmp;
		Vector3 *v;
		if (out == &in) {
			tmp = in;
			v = &tmp;
		} else {
			v = (Vector3 *)&in;
		}
		out->X = (tm[0][0] * v->X + tm[0][1] * v->Y + tm[0][2] * v->Z + tm[0][3]);
		out->Y = (tm[1][0] * v->X + tm[1][1] * v->Y + tm[1][2] * v->Z + tm[1][3]);
		out->Z = (tm[2][0] * v->X + tm[2][1] * v->Y + tm[2][2] * v->Z + tm[2][3]);
	}

	static __forceinline void Rotate_Vector(const Matrix3D &tm, const Vector3 &in, Vector3 *out)
	{
		Vector3 tmp;
		Vector3 *v;
		if (out == &in) {
			tmp = in;
			v = &tmp;
		} else {
			v = (Vector3 *)&in;
		}
		out->X = (tm[0][0] * v->X + tm[0][1] * v->Y + tm[0][2] * v->Z);
		out->Y = (tm[1][0] * v->X + tm[1][1] * v->Y + tm[1][2] * v->Z);
		out->Z = (tm[2][0] * v->X + tm[2][1] * v->Y + tm[2][2] * v->Z);
	}

	// WWMath's in-place transform (Zero Hour matrix3d.h, the NO_ALLOW_TEMPORARIES form of operator*).
	__forceinline void mulVector3(const Vector3& in, Vector3& out) const
	{
		// note, it's ok for in==out here.
		float x = (Row[0].X * in.X + Row[0].Y * in.Y + Row[0].Z * in.Z + Row[0].W);
		float y = (Row[1].X * in.X + Row[1].Y * in.Y + Row[1].Z * in.Z + Row[1].W);
		float z = (Row[2].X * in.X + Row[2].Y * in.Y + Row[2].Z * in.Z + Row[2].W);
		out.Set(x, y, z);
	}
	void mulVector3(Vector3& inout) const { mulVector3(inout, inout); }

	__forceinline void mulVector3Array(const Vector3* in, Vector3* out, int count) const
	{
		while (count--)
		{
			// we know that in != out, so no temps needed
			out->X = (Row[0].X * in->X + Row[0].Y * in->Y + Row[0].Z * in->Z + Row[0].W);
			out->Y = (Row[1].X * in->X + Row[1].Y * in->Y + Row[1].Z * in->Z + Row[1].W);
			out->Z = (Row[2].X * in->X + Row[2].Y * in->Y + Row[2].Z * in->Z + Row[2].W);
			++in;
			++out;
		}
	}

	// Zero Hour's form: one Vector4::Set per row. Retail's inlined copies (Obj_Look_At 0x007126B0, buildTransformMatrix
	// 0x00712440, Re_Orthogonalize 0x00713810) store the twelve words in the order this gives.
	void Make_Identity()
	{
		Row[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
		Row[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
		Row[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
	}

	// Zero Hour's matrix3d.h axis and translation getters (three columns / the last column; plain inline, not WWINLINE). evidence: retail inlines the
	// by-value forms with no out-of-line copy (ActiveBody 0x004BFB1C reads column 0 as [m+0x8],[m+0x18],[m+0x28] and column 1 as
	// [m+0xC],[m+0x1C],[m+0x2C]; f14's request), yet W3DSailModelDraw's unit emits out-of-line copies of Get_X_Vector 0x000D085E and
	// Get_Y_Vector 0x000D087D (calls it makes without expansion), so they are not forced; the Z and pointer forms are the same family.
	inline Vector3 Get_X_Vector() const { return Vector3(Row[0][0], Row[1][0], Row[2][0]); }
	inline Vector3 Get_Y_Vector() const { return Vector3(Row[0][1], Row[1][1], Row[2][1]); }
	inline Vector3 Get_Z_Vector() const { return Vector3(Row[0][2], Row[1][2], Row[2][2]); }
	inline void Get_X_Vector(Vector3 *set_x) const { set_x->Set(Row[0][0], Row[1][0], Row[2][0]); }
	inline void Get_Y_Vector(Vector3 *set_y) const { set_y->Set(Row[0][1], Row[1][1], Row[2][1]); }
	inline void Get_Z_Vector(Vector3 *set_z) const { set_z->Set(Row[0][2], Row[1][2], Row[2][2]); }
	inline Vector3 Get_Translation() const { return Vector3(Row[0][3], Row[1][3], Row[2][3]); }
	inline void Get_Translation(Vector3 *set) const { set->X = Row[0][3]; set->Y = Row[1][3]; set->Z = Row[2][3]; }
	float Get_X_Translation() const { return Row[0][3]; }
	float Get_Y_Translation() const { return Row[1][3]; }
	float Get_Z_Translation() const { return Row[2][3]; }
	// Zero Hour's WWINLINE translation setters (Thing::setPosition 0x0030AA80 stores +0x14/+0x24/+0x34 in place).
	WWINLINE void Set_X_Translation(float x) { Row[0][3] = x; }
	WWINLINE void Set_Y_Translation(float y) { Row[1][3] = y; }
	WWINLINE void Set_Z_Translation(float z) { Row[2][3] = z; }

    void Set_Translation(const Vector3 &v) { Row[0][3]=v.X;Row[1][3]=v.Y;Row[2][3]=v.Z; }
	// Post-multiplying translations and rotations (Zero Hour's matrix3d.h).
	void Translate(const Vector3 &t);
	void mul(const Matrix3D &a, const Matrix3D &b);
	void Rotate_X(float theta);
	void Rotate_X(float s, float c);
	void Rotate_Y(float theta);
	void Rotate_Y(float s, float c);
	void Rotate_Z(float theta);
	void Rotate_Z(float s, float c);
	// Pre-multiplying the rotation part (Zero Hour's matrix3d.h): retail expands all of them in place.
	void In_Place_Pre_Rotate_X(float theta);	// the map's out-of-line copy: 0x000E9E27
	void In_Place_Pre_Rotate_Y(float theta);	// the map's out-of-line copy: 0x000E9EF3
	void In_Place_Pre_Rotate_Z(float theta);
	void In_Place_Pre_Rotate_X(float s, float c);
	void In_Place_Pre_Rotate_Y(float s, float c);
	void In_Place_Pre_Rotate_Z(float s, float c);

	Vector4 Row[3];		// 0x0
};
// SIZE 0x30

// Elementwise comparison over the three rows (HLodClass::Set_Transform 0x0019B6F0 expands it: four
// columns per row, the first unequal element ends it).
__forceinline bool operator==(const Matrix3D &a, const Matrix3D &b)
{
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 4; col++) {
			if (a[row][col] != b[row][col]) {
				return false;
			}
		}
	}
	return true;
}

__forceinline bool operator!=(const Matrix3D &a, const Matrix3D &b)
{
	return !(a == b);
}

inline void Matrix3D::Translate(const Vector3 &t)
{
	Row[0][3] += Row[0][0] * t[0] + Row[0][1] * t[1] + Row[0][2] * t[2];
	Row[1][3] += Row[1][0] * t[0] + Row[1][1] * t[1] + Row[1][2] * t[2];
	Row[2][3] += Row[2][0] * t[0] + Row[2][1] * t[1] + Row[2][2] * t[2];
}

inline void Matrix3D::Rotate_X(float s, float c)
{
	float tmp1, tmp2;
	tmp1 = Row[0][1]; tmp2 = Row[0][2];
	Row[0][1] = (float)( c*tmp1 + s*tmp2);
	Row[0][2] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[1][1]; tmp2 = Row[1][2];
	Row[1][1] = (float)( c*tmp1 + s*tmp2);
	Row[1][2] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[2][1]; tmp2 = Row[2][2];
	Row[2][1] = (float)( c*tmp1 + s*tmp2);
	Row[2][2] = (float)(-s*tmp1 + c*tmp2);
}

inline void Matrix3D::Rotate_Y(float s, float c)
{
	float tmp1, tmp2;
	tmp1 = Row[0][0]; tmp2 = Row[0][2];
	Row[0][0] = (float)(c*tmp1 - s*tmp2);
	Row[0][2] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[1][0]; tmp2 = Row[1][2];
	Row[1][0] = (float)(c*tmp1 - s*tmp2);
	Row[1][2] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[2][0]; tmp2 = Row[2][2];
	Row[2][0] = (float)(c*tmp1 - s*tmp2);
	Row[2][2] = (float)(s*tmp1 + c*tmp2);
}

inline void Matrix3D::Rotate_Z(float s, float c)
{
	float tmp1, tmp2;
	tmp1 = Row[0][0]; tmp2 = Row[0][1];
	Row[0][0] = (float)( c*tmp1 + s*tmp2);
	Row[0][1] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[1][0]; tmp2 = Row[1][1];
	Row[1][0] = (float)( c*tmp1 + s*tmp2);
	Row[1][1] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[2][0]; tmp2 = Row[2][1];
	Row[2][0] = (float)( c*tmp1 + s*tmp2);
	Row[2][1] = (float)(-s*tmp1 + c*tmp2);
}

// The angle forms and the in-place pre-rotations are Zero Hour's WWINLINE members: retail expands them at the call
// sites (Obj_Look_At 0x007126B0, buildTransformMatrix 0x00712440, ToppleUpdate::update 0x004A878F) and the angle
// forms call the CRT sin/cos through their thunks, which is what cosf/sinf are without /Oi.
WWINLINE void Matrix3D::Rotate_X(float theta)
{
	float tmp1, tmp2;
	float s, c;
	s = sinf(theta);
	c = cosf(theta);
	tmp1 = Row[0][1]; tmp2 = Row[0][2];
	Row[0][1] = (float)( c*tmp1 + s*tmp2);
	Row[0][2] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[1][1]; tmp2 = Row[1][2];
	Row[1][1] = (float)( c*tmp1 + s*tmp2);
	Row[1][2] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[2][1]; tmp2 = Row[2][2];
	Row[2][1] = (float)( c*tmp1 + s*tmp2);
	Row[2][2] = (float)(-s*tmp1 + c*tmp2);
}

WWINLINE void Matrix3D::Rotate_Y(float theta)
{
	float tmp1, tmp2;
	float s, c;
	s = sinf(theta);
	c = cosf(theta);
	tmp1 = Row[0][0]; tmp2 = Row[0][2];
	Row[0][0] = (float)(c*tmp1 - s*tmp2);
	Row[0][2] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[1][0]; tmp2 = Row[1][2];
	Row[1][0] = (float)(c*tmp1 - s*tmp2);
	Row[1][2] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[2][0]; tmp2 = Row[2][2];
	Row[2][0] = (float)(c*tmp1 - s*tmp2);
	Row[2][2] = (float)(s*tmp1 + c*tmp2);
}

WWINLINE void Matrix3D::Rotate_Z(float theta)
{
	float tmp1, tmp2;
	float c, s;
	c = cosf(theta);
	s = sinf(theta);
	tmp1 = Row[0][0]; tmp2 = Row[0][1];
	Row[0][0] = (float)( c*tmp1 + s*tmp2);
	Row[0][1] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[1][0]; tmp2 = Row[1][1];
	Row[1][0] = (float)( c*tmp1 + s*tmp2);
	Row[1][1] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[2][0]; tmp2 = Row[2][1];
	Row[2][0] = (float)( c*tmp1 + s*tmp2);
	Row[2][1] = (float)(-s*tmp1 + c*tmp2);
}

WWINLINE void Matrix3D::In_Place_Pre_Rotate_X(float theta)
{
	float tmp1, tmp2;
	float c, s;
	c = cosf(theta);
	s = sinf(theta);
	tmp1 = Row[1][0]; tmp2 = Row[2][0];
	Row[1][0] = (float)(c*tmp1 - s*tmp2);
	Row[2][0] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[1][1]; tmp2 = Row[2][1];
	Row[1][1] = (float)(c*tmp1 - s*tmp2);
	Row[2][1] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[1][2]; tmp2 = Row[2][2];
	Row[1][2] = (float)(c*tmp1 - s*tmp2);
	Row[2][2] = (float)(s*tmp1 + c*tmp2);
}

WWINLINE void Matrix3D::In_Place_Pre_Rotate_Y(float theta)
{
	float tmp1, tmp2;
	float c, s;
	c = cosf(theta);
	s = sinf(theta);
	tmp1 = Row[0][0]; tmp2 = Row[2][0];
	Row[0][0] = (float)( c*tmp1 + s*tmp2);
	Row[2][0] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[0][1]; tmp2 = Row[2][1];
	Row[0][1] = (float)( c*tmp1 + s*tmp2);
	Row[2][1] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[0][2]; tmp2 = Row[2][2];
	Row[0][2] = (float)( c*tmp1 + s*tmp2);
	Row[2][2] = (float)(-s*tmp1 + c*tmp2);
}

WWINLINE void Matrix3D::In_Place_Pre_Rotate_Z(float theta)
{
	float tmp1, tmp2;
	float c, s;
	c = cosf(theta);
	s = sinf(theta);
	tmp1 = Row[0][0]; tmp2 = Row[1][0];
	Row[0][0] = (float)(c*tmp1 - s*tmp2);
	Row[1][0] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[0][1]; tmp2 = Row[1][1];
	Row[0][1] = (float)(c*tmp1 - s*tmp2);
	Row[1][1] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[0][2]; tmp2 = Row[1][2];
	Row[0][2] = (float)(c*tmp1 - s*tmp2);
	Row[1][2] = (float)(s*tmp1 + c*tmp2);
}

WWINLINE void Matrix3D::In_Place_Pre_Rotate_X(float s, float c)
{
	float tmp1, tmp2;
	tmp1 = Row[1][0]; tmp2 = Row[2][0];
	Row[1][0] = (float)(c*tmp1 - s*tmp2);
	Row[2][0] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[1][1]; tmp2 = Row[2][1];
	Row[1][1] = (float)(c*tmp1 - s*tmp2);
	Row[2][1] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[1][2]; tmp2 = Row[2][2];
	Row[1][2] = (float)(c*tmp1 - s*tmp2);
	Row[2][2] = (float)(s*tmp1 + c*tmp2);
}

WWINLINE void Matrix3D::In_Place_Pre_Rotate_Y(float s, float c)
{
	float tmp1, tmp2;
	tmp1 = Row[0][0]; tmp2 = Row[2][0];
	Row[0][0] = (float)( c*tmp1 + s*tmp2);
	Row[2][0] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[0][1]; tmp2 = Row[2][1];
	Row[0][1] = (float)( c*tmp1 + s*tmp2);
	Row[2][1] = (float)(-s*tmp1 + c*tmp2);
	tmp1 = Row[0][2]; tmp2 = Row[2][2];
	Row[0][2] = (float)( c*tmp1 + s*tmp2);
	Row[2][2] = (float)(-s*tmp1 + c*tmp2);
}

WWINLINE void Matrix3D::In_Place_Pre_Rotate_Z(float s, float c)
{
	float tmp1, tmp2;
	tmp1 = Row[0][0]; tmp2 = Row[1][0];
	Row[0][0] = (float)(c*tmp1 - s*tmp2);
	Row[1][0] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[0][1]; tmp2 = Row[1][1];
	Row[0][1] = (float)(c*tmp1 - s*tmp2);
	Row[1][1] = (float)(s*tmp1 + c*tmp2);
	tmp1 = Row[0][2]; tmp2 = Row[1][2];
	Row[0][2] = (float)(c*tmp1 - s*tmp2);
	Row[1][2] = (float)(s*tmp1 + c*tmp2);
}

WWINLINE float submul(const Vector4 &row, float tmp1, float tmp2, float tmp3)
{
	return row.X * tmp1 + row.Y * tmp2 + row.Z * tmp3;
}

// does "this = A * B"
WWINLINE void Matrix3D::mul(const Matrix3D &A, const Matrix3D &B)
{
	float tmp1, tmp2, tmp3;

	tmp1 = B.Row[0].X;
	tmp2 = B.Row[1].X;
	tmp3 = B.Row[2].X;

	this->Row[0].X = submul(A.Row[0], tmp1, tmp2, tmp3);
	this->Row[1].X = submul(A.Row[1], tmp1, tmp2, tmp3);
	this->Row[2].X = submul(A.Row[2], tmp1, tmp2, tmp3);

	tmp1 = B.Row[0].Y;
	tmp2 = B.Row[1].Y;
	tmp3 = B.Row[2].Y;

	this->Row[0].Y = submul(A.Row[0], tmp1, tmp2, tmp3);
	this->Row[1].Y = submul(A.Row[1], tmp1, tmp2, tmp3);
	this->Row[2].Y = submul(A.Row[2], tmp1, tmp2, tmp3);

	tmp1 = B.Row[0].Z;
	tmp2 = B.Row[1].Z;
	tmp3 = B.Row[2].Z;

	this->Row[0].Z = submul(A.Row[0], tmp1, tmp2, tmp3);
	this->Row[1].Z = submul(A.Row[1], tmp1, tmp2, tmp3);
	this->Row[2].Z = submul(A.Row[2], tmp1, tmp2, tmp3);

	tmp1 = B.Row[0].W;
	tmp2 = B.Row[1].W;
	tmp3 = B.Row[2].W;

	this->Row[0].W = submul(A.Row[0], tmp1, tmp2, tmp3) + A.Row[0].W;
	this->Row[1].W = submul(A.Row[1], tmp1, tmp2, tmp3) + A.Row[1].W;
	this->Row[2].W = submul(A.Row[2], tmp1, tmp2, tmp3) + A.Row[2].W;
}

#endif
