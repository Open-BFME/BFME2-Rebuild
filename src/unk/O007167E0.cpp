// cl: /O2 /G7 /arch:SSE
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include "WWMath/Matrix3.h"
#include "WWMath/Matrix3D.h"
#include "WWMath/Quaternion.h"
#include "WWMath/wwmath.h"

#define WWMATH_EPSILON 0.0001f

// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=O007167E0 name-evidence=L
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x007167E0 size=56 name=?Make_Identity@Matrix3@@QAEXXZ evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
void Matrix3::Make_Identity(void)
{
	Row[0].Set(1.0f, 0.0f, 0.0f);
	Row[1].Set(0.0f, 1.0f, 0.0f);
	Row[2].Set(0.0f, 0.0f, 1.0f);
}

// FUNCTION: BFME2 0x00716820 size=103 name=??0Matrix3@@QAE@MMMMMMMMM@Z evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
Matrix3::Matrix3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
{
	Row[0].Set(m11, m12, m13);
	Row[1].Set(m21, m22, m23);
	Row[2].Set(m31, m32, m33);
}

inline void Matrix3::Assign_Rotation(const Matrix3D & m)
{
	Row[0].Set(m[0][0],m[0][1],m[0][2]);
	Row[1].Set(m[1][0],m[1][1],m[1][2]);
	Row[2].Set(m[2][0],m[2][1],m[2][2]);
}

// FUNCTION: BFME2 0x00716890 size=85 name=??0Matrix3@@QAE@ABVMatrix3D@@@Z evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
Matrix3::Matrix3(const Matrix3D & m)
{
	Assign_Rotation(m);
}

Matrix3 &Matrix3::operator=(const Matrix3D &m)
{
	Assign_Rotation(m);
	return *this;
}

// FUNCTION: BFME2 0x007168F0 size=83 name=?Set@Matrix3@@QAEXABVMatrix3D@@@Z evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
void Matrix3::Set(const Matrix3D &m)
{
	Assign_Rotation(m);
}

// FUNCTION: BFME2 0x00716950 size=205 name=?Set@Matrix3@@QAEXABVQuaternion@@@Z evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
void Matrix3::Set(const Quaternion &q)
{
	Row[0][0] = (float)(1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]));
	Row[0][1] = (float)(2.0 * (q[0] * q[1] - q[2] * q[3]));
	Row[0][2] = (float)(2.0 * (q[2] * q[0] + q[1] * q[3]));

	Row[1][0] = (float)(2.0 * (q[0] * q[1] + q[2] * q[3]));
	Row[1][1] = (float)(1.0 - 2.0f * (q[2] * q[2] + q[0] * q[0]));
	Row[1][2] = (float)(2.0 * (q[1] * q[2] - q[0] * q[3]));

	Row[2][0] = (float)(2.0 * (q[2] * q[0] - q[1] * q[3]));
	Row[2][1] = (float)(2.0 * (q[1] * q[2] + q[0] * q[3]));
	Row[2][2] = (float)(1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]));
}

// FUNCTION: BFME2 0x00716A20 size=393 name=?Multiply@Matrix3@@SAXABVMatrix3D@@ABV1@PAV1@@Z evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
void Matrix3::Multiply(const Matrix3D &a, const Matrix3 &b, Matrix3 *res)
{
#define ROWCOL(i, j) a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j]

	(*res)[0][0] = ROWCOL(0, 0);
	(*res)[0][1] = ROWCOL(0, 1);
	(*res)[0][2] = ROWCOL(0, 2);

	(*res)[1][0] = ROWCOL(1, 0);
	(*res)[1][1] = ROWCOL(1, 1);
	(*res)[1][2] = ROWCOL(1, 2);

	(*res)[2][0] = ROWCOL(2, 0);
	(*res)[2][1] = ROWCOL(2, 1);
	(*res)[2][2] = ROWCOL(2, 2);

#undef ROWCOL
}

// FUNCTION: BFME2 0x00716BB0 size=393 name=?Multiply@Matrix3@@SAXABV1@ABVMatrix3D@@PAV1@@Z evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
void Matrix3::Multiply(const Matrix3 &a, const Matrix3D &b, Matrix3 *res)
{
#define ROWCOL(i, j) a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j]

	(*res)[0][0] = ROWCOL(0, 0);
	(*res)[0][1] = ROWCOL(0, 1);
	(*res)[0][2] = ROWCOL(0, 2);

	(*res)[1][0] = ROWCOL(1, 0);
	(*res)[1][1] = ROWCOL(1, 1);
	(*res)[1][2] = ROWCOL(1, 2);

	(*res)[2][0] = ROWCOL(2, 0);
	(*res)[2][1] = ROWCOL(2, 1);
	(*res)[2][2] = ROWCOL(2, 2);

#undef ROWCOL
}

// FUNCTION: BFME2 0x00716D40 size=490 name=?Multiply@Matrix3@@SAXABV1@0PAV1@@Z evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
void Matrix3::Multiply(const Matrix3 &A, const Matrix3 &B, Matrix3 *set_res)
{
	Matrix3 tmp;
	const Matrix3 *Aptr;
	float tmp1, tmp2, tmp3;

	// copy A first when the result aliases it
	if (set_res == &A) {
		tmp = A;
		Aptr = &tmp;
	} else {
		Aptr = &A;
	}

	tmp1 = B[0][0];
	tmp2 = B[1][0];
	tmp3 = B[2][0];

	(*set_res)[0][0] = (float)((*Aptr)[0][0] * tmp1 + (*Aptr)[0][1] * tmp2 + (*Aptr)[0][2] * tmp3);
	(*set_res)[1][0] = (float)((*Aptr)[1][0] * tmp1 + (*Aptr)[1][1] * tmp2 + (*Aptr)[1][2] * tmp3);
	(*set_res)[2][0] = (float)((*Aptr)[2][0] * tmp1 + (*Aptr)[2][1] * tmp2 + (*Aptr)[2][2] * tmp3);

	tmp1 = B[0][1];
	tmp2 = B[1][1];
	tmp3 = B[2][1];

	(*set_res)[0][1] = (float)((*Aptr)[0][0] * tmp1 + (*Aptr)[0][1] * tmp2 + (*Aptr)[0][2] * tmp3);
	(*set_res)[1][1] = (float)((*Aptr)[1][0] * tmp1 + (*Aptr)[1][1] * tmp2 + (*Aptr)[1][2] * tmp3);
	(*set_res)[2][1] = (float)((*Aptr)[2][0] * tmp1 + (*Aptr)[2][1] * tmp2 + (*Aptr)[2][2] * tmp3);

	tmp1 = B[0][2];
	tmp2 = B[1][2];
	tmp3 = B[2][2];

	(*set_res)[0][2] = (float)((*Aptr)[0][0] * tmp1 + (*Aptr)[0][1] * tmp2 + (*Aptr)[0][2] * tmp3);
	(*set_res)[1][2] = (float)((*Aptr)[1][0] * tmp1 + (*Aptr)[1][1] * tmp2 + (*Aptr)[1][2] * tmp3);
	(*set_res)[2][2] = (float)((*Aptr)[2][0] * tmp1 + (*Aptr)[2][1] * tmp2 + (*Aptr)[2][2] * tmp3);
}

// FUNCTION: BFME2 0x00716F30 size=480 name=?Is_Orthogonal@Matrix3@@QBEHXZ evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
int Matrix3::Is_Orthogonal(void) const
{
	Vector3 x(Row[0].X, Row[0].Y, Row[0].Z);
	Vector3 y(Row[1].X, Row[1].Y, Row[1].Z);
	Vector3 z(Row[2].X, Row[2].Y, Row[2].Z);

	if (Vector3::Dot_Product(x, y) > WWMATH_EPSILON) return 0;
	if (Vector3::Dot_Product(y, z) > WWMATH_EPSILON) return 0;
	if (Vector3::Dot_Product(z, x) > WWMATH_EPSILON) return 0;

	if (WWMath::Fabs(x.Length() - 1.0f) > WWMATH_EPSILON) return 0;
	if (WWMath::Fabs(y.Length() - 1.0f) > WWMATH_EPSILON) return 0;
	if (WWMath::Fabs(z.Length() - 1.0f) > WWMATH_EPSILON) return 0;

	return 1;
}

// FUNCTION: BFME2 0x00717110 size=633 name=?Re_Orthogonalize@Matrix3@@QAEXXZ evidence=N:ledger tier=B hypothesis:mm1-fragment-path-elsewhere
void Matrix3::Re_Orthogonalize(void)
{
	Vector3 x(Row[0][0], Row[0][1], Row[0][2]);
	Vector3 y(Row[1][0], Row[1][1], Row[1][2]);
	Vector3 z;

	Vector3::Cross_Product(x, y, &z);
	Vector3::Cross_Product(z, x, &y);

	float len = x.Length();
	if (len < WWMATH_EPSILON) {
		Make_Identity();
		return;
	} else {
		float ook = 1.0f / len;
		x.X = x.X * ook;
		x.Y = x.Y * ook;
		x.Z = x.Z * ook;
	}

	len = y.Length();
	if (len < WWMATH_EPSILON) {
		Make_Identity();
		return;
	} else {
		float ook = 1.0f / len;
		y.X = y.X * ook;
		y.Y = y.Y * ook;
		y.Z = y.Z * ook;
	}

	len = z.Length();
	if (len < WWMATH_EPSILON) {
		Make_Identity();
		return;
	} else {
		float ook = 1.0f / len;
		z.X = z.X * ook;
		z.Y = z.Y * ook;
		z.Z = z.Z * ook;
	}

	Row[0][0] = x.X;
	Row[0][1] = x.Y;
	Row[0][2] = x.Z;

	Row[1][0] = y.X;
	Row[1][1] = y.Y;
	Row[1][2] = y.Z;

	Row[2][0] = z.X;
	Row[2][1] = z.Y;
	Row[2][2] = z.Z;
}

