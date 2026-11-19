// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// AABTIntersectStruct.h: the scratchpad record of colmathaabtri.cpp's Intersection_Test (Zero Hour keeps it local to
// that unit). Retail: SIZE 0xE0, Box +0xD8 and Tri +0xDC set by the constructor 0x00724F30, Init 0x00724F50.
#pragma once
#ifndef BFME2_AABTINTERSECTSTRUCT_H
#define BFME2_AABTINTERSECTSTRUCT_H

#include "WWMath/Vector3.h"
#include "WWMath/AABoxClass.h"
#include "WWMath/TriClass.h"

/*
** AABTIntersectStruct
** Scratchpad variables for the AABox-Triangle intersection functions.  One instance
** of this structure will be used for all of the local variables and its pointer will be 
** handed of to various inline functions for the axis tests.
** Note that much of the code needs the un-normalized triangle normal.  For this reason,
** I have to compute N rather than copying it from the triangle.  (commenting this to
** avoid re-generating a difficult to find bug that I had)
*/

struct AABTIntersectStruct
{
	AABTIntersectStruct(void);

	void Init(const AABoxClass &box,const TriClass &tri);


	Vector3					D;						// Vector from the center of the box to v0
	float						AE[3][3];			// Dot products of the Basis vectors and edges
	float						AN[3];				// Dot products of the Basis vectors and the normal
	Vector3					AxE[3][3];			// Cross produts of the Basis vectors and edges

	Vector3					E[3];					// edge vectors for the triangle 
	Vector3					N;						// normal (NOT normalized!!!)

	const AABoxClass *	Box;
	const TriClass *		Tri;

private:
	
	// not implemented
	AABTIntersectStruct(const AABTIntersectStruct &);
	AABTIntersectStruct & operator = (const AABTIntersectStruct &);
};

#endif
