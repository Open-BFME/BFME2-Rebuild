// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// LineSegClass.h: WWMath's line segment, the start and end points plus the values derived from them.
// Target facts (retail game.dat): LineSegClass::Set_Random 0x007195C0 stores the two end points at +0x00 and
// +0x0C, then the difference P1 - P0 at +0x18, a copy of it at +0x24 that it scales to unit length, and the
// segment length at +0x30. The collision routines (CollisionMath::Collide 0x00713FE0, 0x00714060) read the
// start point and the difference. SIZE 0x34.
#pragma once
#ifndef BFME2_LINESEGCLASS_H
#define BFME2_LINESEGCLASS_H

#include "WWMath/Vector3.h"

class Matrix3D;

class LineSegClass
{
public:
	LineSegClass(void) {}
	LineSegClass(const Vector3 &p0, const Vector3 &p1) : P0(p0), P1(p1) { recalculate(); }

	// Zero Hour's lineseg.h inlines; retail keeps COMDAT copies at 0x0006BEF9 (Set), 0x0006A1E4 (recalculate)
	// and 0x0013B0A0 (Compute_Point).
	void Set(const Vector3 &p0, const Vector3 &p1) { P0 = p0; P1 = p1; recalculate(); }
	void Compute_Point(float t, Vector3 *set) const { Vector3::Add(P0, t * DP, set); }

	void Set(const LineSegClass &that, const Matrix3D &tm);					// 0x00718F70
	void Set_Random(const Vector3 &min, const Vector3 &max);				// 0x007195C0
	Vector3 Find_Point_Closest_To(const Vector3 &pos) const;				// 0x007191D0
	bool Find_Intersection(const LineSegClass &other_line, Vector3 *p1, float *fraction1, Vector3 *p2, float *fraction2) const;	// 0x007192A0

protected:
	// Protected in retail (map ?recalculate@LineSegClass@@IAEXXZ at 0x0006A1E4).
	void recalculate(void) { DP = P1 - P0; Dir = DP; Dir.Normalize(); Length = DP.Length(); }

public:
	Vector3 P0;		// +0x00: start point
	Vector3 P1;		// +0x0C: end point
	Vector3 DP;		// +0x18: P1 - P0
	Vector3 Dir;		// +0x24: DP scaled to unit length
	Real Length;		// +0x30: length of DP
};
typedef char LineSegClassSize[(sizeof(LineSegClass) == 0x34) ? 1 : -1];

#endif
