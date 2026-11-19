// intersec.h: WW3D2's ray intersection request (IntersectionClass) and its result record.
// Layout from Zero Hour's intersec.h, checked against retail's RenderObjClass::Intersect 0x0013C3B0, which reads
// RayLocation/RayDirection/IntersectionNormal at +0x04/+0x08/+0x0C and MaxDistance at +0x1C of the request, and
// writes the result through offsets 0x00 (IntersectedRenderObject), 0x08 (ModelMatrix), 0x44 (Intersection),
// 0x5C (Intersects) and 0x64 (IntersectionType). The request's own result record starts at +0x20.
// Only the members a recovered row reads are declared; the static-pointer constructor, Copy_Results and the
// polygon helpers wait for their callers.
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_INTERSEC_H
#define BFME2_INTERSEC_H

#include <math.h>
#include "WWMath/Matrix3D.h"
#include "WWMath/SphereClass.h"
#include "WWMath/Vector3.h"

class RenderObjClass;

class IntersectionResultClass
{
public:
	RenderObjClass *IntersectedRenderObject;	// +0x00
	unsigned short IntersectedPolygon;		// +0x04
	Matrix3D ModelMatrix;				// +0x08: transforms mesh normals from model coordinates
	Vector3 ModelLocation;				// +0x38
	Vector3 Intersection;				// +0x44: location of the intersection
	float Range;					// +0x50
	float Alpha;					// +0x54
	float Beta;					// +0x58
	bool Intersects;				// +0x5C
	int CollisionType;				// +0x60
	enum INTERSECTION_TYPE { NONE = 0, GENERIC, POLYGON } IntersectionType;		// +0x64
};
// SIZE 0x68
typedef char IntersectionResultClassSize[(sizeof(IntersectionResultClass) == 0x68) ? 1 : -1];

class IntersectionClass
{
public:
	virtual ~IntersectionClass() {}				// +0x00 vptr

	Vector3 *RayLocation;					// +0x04
	Vector3 *RayDirection;					// +0x08
	Vector3 *IntersectionNormal;				// +0x0C
	float ScreenX;						// +0x10
	float ScreenY;						// +0x14
	bool InterpolateNormal;					// +0x18
	bool ConvexTest;					// +0x19
	float MaxDistance;					// +0x1C
	IntersectionResultClass Result;				// +0x20

	// Zero Hour's inlines (intersec.h). Retail keeps COMDAT copies at 0x0013B1B0 (Intersect_Sphere_Quick) and 0x0013B260
	// (Intersect_Sphere); RenderObjClass::Intersect_Sphere_Quick 0x0013BD70 expands the quick test and calls the other.
	// Find the range to the intersection of the ray and a sphere: only sets Alpha/Beta/Intersects (first pass test).
	inline bool Intersect_Sphere_Quick(SphereClass &Sphere, IntersectionResultClass *FinalResult)
	{
		// make a vector from the ray origin to the sphere center
		Vector3 sphere_vector(Sphere.Center - *RayLocation);

		// get the dot product between the sphere_vector and the ray vector
		FinalResult->Alpha = Vector3::Dot_Product(sphere_vector, *RayDirection);

		FinalResult->Beta = Sphere.Radius * Sphere.Radius - (Vector3::Dot_Product(sphere_vector, sphere_vector) - FinalResult->Alpha * FinalResult->Alpha);

		if (FinalResult->Beta < 0.0f) {
			return FinalResult->Intersects = false;
		}
		return FinalResult->Intersects = true;
	}

	// This will find the intersection with the sphere and the intersection normal if needed.
	inline bool Intersect_Sphere(SphereClass &Sphere, IntersectionResultClass *FinalResult)
	{
		if (!Intersect_Sphere_Quick(Sphere, FinalResult)) {
			return false;
		}

		// determine range to intersection based on stored alpha/beta values
		float d = sqrtf(FinalResult->Beta);
		FinalResult->Range = FinalResult->Alpha - d;

		if (FinalResult->Range > MaxDistance) return false;

		FinalResult->Intersection = *RayLocation + FinalResult->Range * (*RayDirection);

		if (IntersectionNormal != 0) {
			(*IntersectionNormal) = FinalResult->Intersection - Sphere.Center;
		}
		return true;
	}
};
// SIZE 0x88
typedef char IntersectionClassSize[(sizeof(IntersectionClass) == 0x88) ? 1 : -1];

#endif
