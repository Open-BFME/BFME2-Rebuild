// SphereClass.h: WWMath's bounding sphere (WWVegas/WWMath/sphere.h).
// Target facts: Center (Vector3) +0, Radius +0xC: Init 0x00069146 copies the
// centre's three words and stores the radius at +0xC; Null3DObjClass's
// Get_Obj_Space_Bounding_Sphere 0x00180FE0 zeroes +0..+8 and sets +0xC.
// Inline as in WWMath: Init, Transform 0x0019AF20 and Add_Sphere 0x0019BE00
// exist in retail only as uncalled copies; Re_Center 0x0006F400 is out of line in W3DScene.cpp. Only the
// members with a verified plain body are declared.
#pragma once
#ifndef BFME2_SPHERECLASS_H
#define BFME2_SPHERECLASS_H

#include "WWMath/Vector3.h"
#include "WWMath/Matrix3D.h"

class SphereClass
{
public:
	SphereClass() {}
	SphereClass(const Vector3 &center, float radius) { Init(center, radius); }

	void Init(const Vector3 &pos, float radius) { Center = pos; Radius = radius; }
	void Add_Sphere(const SphereClass &s);							// 0x0019BE00 (inline; retail's callers expand it)
	void Transform(const Matrix3D &tm);							// 0x0019AF20
	void Re_Center(const Vector3 &center);						// 0x0006F400

	Vector3 Center;		// 0x0
	float Radius;		// 0xC
};
// SIZE 0x10
// Zero Hour's sphere.h bodies: HLodClass::Update_Obj_Space_Bounding_Volumes 0x0019DB50 and CompositeRenderObjClass's
// 0x001A61A0 expand them; the out-of-line copies at 0x0019BE00 and 0x0019AF20 are their COMDATs.
inline void SphereClass::Add_Sphere(const SphereClass &s)
{
	if (s.Radius == 0.0f) return;

	float dist = (s.Center - Center).Length();
	if (dist == 0.0f) {
		Radius = (Radius > s.Radius) ? Radius : s.Radius;
		return;
	}

	float rnew = (dist + Radius + s.Radius) / 2.0f;

	// If rnew is smaller than either of the two sphere radii (it can't be smaller than both of them), the smaller
	// sphere is inside the larger and the result is the larger sphere.
	if (rnew < Radius) {
		// The existing sphere is the result - do nothing.
	} else {
		if (rnew < s.Radius) {
			// The new sphere is the result:
			Init(s.Center, s.Radius);
		} else {
			// Neither sphere is completely inside the other, so rnew is the new radius - calculate the new center
			float lerp = (rnew - Radius) / dist;
			Vector3 center = (s.Center - Center) * lerp + Center;
			Init(center, rnew);
		}
	}
}

inline void SphereClass::Transform(const Matrix3D &tm)
{
	// warning, assumes Orthogonal matrix
	tm.mulVector3(Center, Center);
}

typedef char SphereClassSize[(sizeof(SphereClass) == 0x10) ? 1 : -1];

#endif
