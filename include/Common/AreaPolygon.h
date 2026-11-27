// AreaPolygon.h: the vertex polygon every PolygonTrigger carries at +0x08 (AreaPolygon.cpp).
// Target facts: the constructor builds the vertex vector through reserve, zeroes the two radius words, sets the dirty
// byte and, for the elevated form, clears the elevation after the base constructor. The bounds refresh takes min/max over
// the 8-byte vertices into the bounds rectangle; the accessors refresh first when the dirty byte is set. The ledger names
// every method here under AreaPolygonBase and has a row for the elevated form's parse method but no further members.
#pragma once
#ifndef BFME2_AREAPOLYGON_H
#define BFME2_AREAPOLYGON_H

#include <vector>
#include "Common/GameType.h"
#include "Common/Region2D.h"

class DataChunkInput;

struct Rva0030B72CPoint	// one polygon corner or centre: two floats (the map's name for the vertex type)
{
	Real x;
	Real y;
};

struct BfmeE8	// the map's name for the 8-byte vertex type of the polygon's vertex vector
{
	Real x;
	Real y;
};

struct Rva0030B6A6Range
{
	const BfmeE8 *begin;
	const BfmeE8 *end;
};	// a vertex range, the operands of the vertex-vector equality

struct Rva0030B7C2Point	// the point argument of the containment test (two floats)
{
	Real x;
	Real y;
};

void Rva0030B31ASwap(Region2D *first, Region2D *second);	// swaps two rectangles

class AreaPolygonBase
{
public:
	void setPoint(Int index, const BfmeE8 &point);	// replaces a vertex, or appends one at the end
	void insertPoint(Int index, const BfmeE8 &point);	// inserts a vertex before the index
	void parse(DataChunkInput &input, Int version);	// reads the vertex count and the vertices
	void reserve(Int count);						// grows the vertex vector (two vertices at least)
	void deletePoint(Int index);					// removes one vertex
	void rva0030B3D1();								// refreshes the bounds, radius and centre values from the vertices
	Region2D rva0030B6E3();							// the bounds (refreshed first when dirty)
	Real rva0030B706() const;						// the value at +0x1C
	Real getRadius() const;
	Rva0030B72CPoint rva0030B72C() const;			// the midpoint of the bounds
	void rva0030B76F(AreaPolygonBase *that);		// swaps the two polygons' contents

	_STL::vector<BfmeE8> m_points;		// +0x00 the vertices
	Region2D m_bounds;								// +0x0C
	Real m_unk0x1C;									// +0x1C (written by the refresh, read by rva0030B706)
	Real m_radius;									// +0x20
	Bool m_boundsDirty;								// +0x24
};
typedef char AreaPolygonBaseSize[(sizeof(AreaPolygonBase) == 0x28) ? 1 : -1];

class ElevatedAreaPolygon : public AreaPolygonBase	// the polygon with an elevation; the ledger has no methods for it besides parse
{
public:
	void parse(DataChunkInput &input, Int version);	// the base parse, then the elevation

	void rva0030B812(ElevatedAreaPolygon *that);	// swaps the two polygons, elevation included

	Int m_elevation;								// +0x28 (the trigger's z when there is no terrain)
};
typedef char ElevatedAreaPolygonSize[(sizeof(ElevatedAreaPolygon) == 0x2C) ? 1 : -1];

Bool rva0030B5AF(const Rva0030B7C2Point *point, AreaPolygonBase *polygon);	// the exact test once the bounds admit the point
Bool rva0030B7C2(const Rva0030B7C2Point *point, AreaPolygonBase *polygon);	// whether the point lies inside the polygon

#endif
