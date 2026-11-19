// Region2D.h: SAGE's float rectangle (Zero Hour BaseType.h Region2D).
// Target facts: struct (export ?IsExactlyEqualTo@Region2D@@QBE_NABU1@@Z);
// XferSaveAsText 0x0060DB8C reads lo.x +0, lo.y +4, hi.x +8, hi.y +0xC.
// Special members (exports): the default constructor folds onto 0x0047A6A9 and the
// destructor onto the bare ret 0x000B3FD0, so both are user-declared empty
// inlines; the copy constructor (0x0004254E) and the corner constructor
// (0x00004C06) are COMDAT folds, i.e. inline definitions some callers did not inline.
// width (0x00004C28) and height (0x0004259B) are exported with no direct callers:
// inline. isInside (0x00004C2E), IsExactlyEqualTo (0x00004C65),
// lineSegmentIntersects (0x000062FD) and expandBy (0x00006423) are owned bodies in
// the base-type library run 0x00003571-0x00006538 that callers reach by call:
// out of line.
#pragma once
#ifndef BFME2_REGION2D_H
#define BFME2_REGION2D_H

#include "Common/GameType.h"
#include "Common/Coord2D.h"

struct Region2D
{
	Coord2D lo;		// 0x0
	Coord2D hi;		// 0x8

	Region2D() {}
	Region2D(const Region2D &that) : lo(that.lo), hi(that.hi) {}
	Region2D(const Coord2D &lowerLeft, const Coord2D &upperRight) : lo(lowerLeft), hi(upperRight) {}
	~Region2D() {}

	Real width() const { return hi.x - lo.x; }
	Real height() const { return hi.y - lo.y; }

	Bool isInside(const Coord2D &point) const;
	Bool IsExactlyEqualTo(const Region2D &that) const;
	Bool lineSegmentIntersects(const Coord2D &start, const Coord2D &delta);
	void expandBy(const Coord2D &point);
};
// SIZE 0x10
typedef char Region2DSize[(sizeof(Region2D) == 0x10) ? 1 : -1];

#endif
