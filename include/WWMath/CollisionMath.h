// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// CollisionMath.h: WWMath's static collision and overlap tests (colmath.h).
// Target facts: OverlapType values from the callers: GridCullSystemClass's leaf
// collectors skip an object when Overlap_Test returns 1 (POS, "outside"), and
// the AABox tests 0x0071AB30/0x0071ABB0 return 1, 2 (NEG, inside) or 8 (BOTH).
// The AABox overloads are owned by gridcull.cpp in retail (defined there ahead
// of the collectors, which inline them); the OBBox and Frustum overloads are
// out of line elsewhere (0x00724800, 0x00719A70).
#pragma once
#ifndef BFME2_COLLISIONMATH_H
#define BFME2_COLLISIONMATH_H

class Vector3;
class AABoxClass;
class OBBoxClass;
class FrustumClass;
class SphereClass;
class LineSegClass;
class TriClass;
class AAPlaneClass;
class PlaneClass;
struct CastResultStruct;

class CollisionMath
{
public:
	enum OverlapType
	{
		POS = 0x01,
		NEG = 0x02,
		ON = 0x04,
		BOTH = 0x08,
		OUTSIDE = POS,
		INSIDE = NEG,
		OVERLAPPED = BOTH
	};

	// Fourteen counters cleared together (the constructor 0x00714BA0 and Reset 0x00714B70 are the same stores).
	struct ColmathStatsStruct
	{
		ColmathStatsStruct();					// 0x00714BA0
		void Reset();						// 0x00714B70

		int Counters[14];
	};

	static bool Collide(const LineSegClass &line, const AAPlaneClass &plane, CastResultStruct *result);	// 0x00713FE0
	static bool Collide(const LineSegClass &line, const PlaneClass &plane, CastResultStruct *result);		// 0x00714060
	static bool Collide(const LineSegClass &line, const SphereClass &sphere, CastResultStruct *result);	// 0x00714160
	static bool Collide(const LineSegClass &line, const AABoxClass &box, CastResultStruct *result);	// 0x007146E0
	static bool Collide(const LineSegClass &line, const OBBoxClass &box, CastResultStruct *result);	// 0x00714860
	static bool Collide(const LineSegClass &line, const TriClass &tri, CastResultStruct *result);		// 0x00714510
	static bool Collide(const OBBoxClass &box0, const Vector3 &move0, const OBBoxClass &box1, const Vector3 &move1, CastResultStruct *result);	// 0x0072B5A0
	static bool Collide(const AABoxClass &box, const Vector3 &move, const TriClass &tri, CastResultStruct *result);	// 0x00726790
	static bool Intersection_Test(const AABoxClass &box, const TriClass &tri);	// 0x00725330
	static bool Intersection_Test(const OBBoxClass &box, const TriClass &tri);	// 0x007221D0 (h03: MeshGeometryClass::intersect_obbox_brute_force 0x0016CD30)

	static OverlapType Overlap_Test(const AABoxClass &box, const Vector3 &point);			// 0x0071AB30
	static OverlapType Overlap_Test(const AABoxClass &box, const AABoxClass &box2);			// 0x0071ABB0
	static OverlapType Overlap_Test(const AABoxClass &box, const LineSegClass &line);	// 0x00723870 (h03: RayCollisionTestClass::Cull 0x00197DE0)
	static OverlapType Overlap_Test(const Vector3 &min, const Vector3 &max, const LineSegClass &line);	// inline in Zero Hour: centre = (min + max) * 0.5, extent = (max - min) * 0.5, then the box form (h03)
	static OverlapType Overlap_Test(const OBBoxClass &box, const AABoxClass &box2);			// 0x00724800
	static OverlapType Overlap_Test(const FrustumClass &frustum, const SphereClass &sphere);	// 0x00719A20
	static OverlapType Overlap_Test(const FrustumClass &frustum, const AABoxClass &box);	// 0x00719A70

	// Inline in retail's callers (CameraClass::Cull_Box 0x00135990 expands them per frustum plane); defined in
	// WWMath/colmathplane.h, which only the .cpp files that use them include (Zero Hour's colmathplane.h).
	static OverlapType Overlap_Test(const PlaneClass &plane, const Vector3 &point);
	static OverlapType Overlap_Test(const PlaneClass &plane, const AABoxClass &box);

private:
	static const float COINCIDENCE_EPSILON;	// ?COINCIDENCE_EPSILON@CollisionMath@@0MB, RVA 0x008EFC94
	static OverlapType eval_overlap_mask(int mask);						// 0x00714BD0
};

#endif
