// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// BTCollisionStruct.h: the scratchpad record of colmathaabtri.cpp (Zero Hour keeps it local to that unit).
// Retail's BTCollisionStruct::Init is an out-of-line function in the unit (0x00724D40); one file-static
// instance serves every AABox-triangle collision call.
#pragma once
#ifndef BFME2_BTCOLLISIONSTRUCT_H
#define BFME2_BTCOLLISIONSTRUCT_H

#include "WWMath/Vector3.h"
#include "WWMath/AABoxClass.h"
#include "WWMath/TriClass.h"

// Axes used in Box-Tri tests.
enum
{
	INTERSECTION = 0,
	AXIS_N,					// normal of the triangle
	AXIS_A0,					// first basis vector of the box
	AXIS_A1,					// second basis vector of the box
	AXIS_A2,					// third basis vector of the box

	AXIS_A0E0,				// box0 x edge0...
	AXIS_A1E0,
	AXIS_A2E0,
	AXIS_A0E1,
	AXIS_A1E1,
	AXIS_A2E1,
	AXIS_A0E2,
	AXIS_A1E2,
	AXIS_A2E2
};

/*
** BoxTriColStruct
** Scratchpad variables for the AABox-Triangle collision detection functions.  One instance
** of this structure will be used for all of the local variables and its pointer will be 
** handed of to various inline functions for the axis tests.
** Note that much of the code needs the un-normalized triangle normal.  For this reason,
** I have to compute N rather than copying it from the triangle.  (commenting this to
** avoid re-generating a difficult to find bug that I had)
*/

struct BTCollisionStruct
{
	BTCollisionStruct(void) {}

	void Init(const AABoxClass &box,const Vector3 &move,const TriClass &tri,const Vector3 &trimove);

	bool						StartBad;			// Inital configuration is intersecting?
	float						MaxFrac;				// Longest move allowed so far

	int						AxisId;				// Last separating axis
	int						Side;					// which side of the interval
	int						Point;				// Index of the "closest" triangle point (or one of them)
	
	int						TestAxisId;			// Axis 'id' we're working on
	int						TestSide;			// Was the axis we're working on flipped
	int						TestPoint;			// Index of the closest vertex
	Vector3					TestAxis;			// Axis we're working on

	Vector3					D;						// Vector from the center of the box to v0
	Vector3					Move;					// Move vector relative to stationary triangle
	float						AE[3][3];			// Dot products of the Basis vectors and edges
	float						AN[3];				// Dot products of the Basis vectors and the normal
	Vector3					AxE[3][3];			// Cross products of the Basis vectors and edges

	Vector3					E[3];					// edge vectors for the triangle 
	Vector3					N;						// normal (NOT normalized!!!)
	Vector3					FinalD;				// Vector from center of box to v0 at end of move

	const AABoxClass *	Box;
	const TriClass *		Tri;
	const Vector3 *		BoxMove;
	const Vector3 *		TriMove;

private:
	
	// not implemented
	BTCollisionStruct(const BTCollisionStruct &);
	BTCollisionStruct & operator = (const BTCollisionStruct &);
};

#endif
