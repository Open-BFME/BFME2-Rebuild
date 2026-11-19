// pivot.h: WW3D2's skeleton bone record (PivotClass, the element of HTreeClass's pivot array).
// Target facts (retail game.dat), from the constructor 0x00197830 and the copy constructor 0x001978A0:
// the record is SIZE 0x58: Name +0 (16 bytes, cleared at its first byte), Parent +0x10, the base transform
// +0x14 and the current transform +0x30 (each a decomposed transform: an identity quaternion, then a zero
// translation, 0x1C bytes), the pivot's own index +0x4C, a visibility byte +0x50 (set), and a fade float
// +0x54 (1.0). HTreeClass::Scale multiplies the translation at +0x24 (the base transform's).
#pragma once
#ifndef BFME2_PIVOT_H
#define BFME2_PIVOT_H

#include "WWMath/Quaternion.h"
#include "WWMath/Vector3.h"
#include "WWMath/Matrix3D.h"

struct PivotTransform
{
	PivotTransform() {}
	PivotTransform(const Quaternion &q, const Vector3 &p) : Rotation(q), Translation(p) {}
	PivotTransform &operator=(const PivotTransform &t) { Rotation = t.Rotation; Translation = t.Translation; return *this; }	// the copy constructor 0x001978A0 copies each record through the inline assignments

	// Zero Hour's Matrix3D-based pivot transform, kept as a rotation and a translation in BFME 2 (HTreeClass::Base_Update 0x001628A0 expands all
	// of the following in place; the product of two transforms is the parent's rotation applied to the child's translation plus the parent's).
	PivotTransform &operator=(const Matrix3D &m)
	{
		Rotation = Build_Quaternion(m);
		Translation.X = m[0][3];
		Translation.Y = m[1][3];
		Translation.Z = m[2][3];
		return *this;
	}
	static void Multiply(const PivotTransform &a, const PivotTransform &b, PivotTransform *res)
	{
		Vector3 tmp;
		a.Rotation.Rotate_Vector(b.Translation, &tmp);
		res->Translation = tmp + a.Translation;
		res->Rotation = a.Rotation * b.Rotation;
	}
	// Post-multiply by another transform (Capture_Update 0x001622B0).
	// evidence: retail 0x001622B0 (HTreeClass::Capture_Update, 1335 bytes) has no call: both PostMul sites expand it, and no plainer form reproduces it (a plain inline compiles out of line).
	__forceinline void PostMul(const PivotTransform &t)
	{
		Vector3 tmp;
		Rotation.Rotate_Vector(t.Translation, &tmp);
		Translation += tmp;
		Rotation.PostMultiply(t.Rotation);
	}

	// The identity transform: HTreeClass::Init_Default 0x001666D0 and read_pivots 0x001614F0 store
	// (0, 0, 0, 1) and a zero translation into each of a root pivot's two transforms.
	void Make_Identity(void) { Rotation.Make_Identity(); Translation.Set(0.0f, 0.0f, 0.0f); }

	Quaternion Rotation;			// +0x00
	Vector3 Translation;			// +0x10
};
// SIZE 0x1C

struct PivotClass
{
	PivotClass();				// 0x00197830
	PivotClass(const PivotClass &that);	// 0x001978A0
	// The copy constructor 0x001978A0 copies Parent, both transforms (the quaternion through inlined
	// memberwise assignments), Index, IsVisible, Fade and last the 16 name bytes; not written yet.
	PivotClass &operator=(const PivotClass &src);	// 0x00197950 (HTreeClass's copy constructor calls it)

	char Name[16];				// +0x00
	PivotClass *Parent;			// +0x10
	PivotTransform Base;			// +0x14
	PivotTransform Current;			// +0x30
	int Index;				// +0x4C
	bool IsVisible;				// +0x50
	float Fade;				// +0x54
};
// SIZE 0x58

#endif
