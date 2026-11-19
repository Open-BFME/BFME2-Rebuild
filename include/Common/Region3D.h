// Region3D.h: SAGE's float box (Zero Hour BaseType.h Region3D).
// Target facts: struct (Xfer::operator==(Region3D&) mangles AAURegion3D);
// XferSaveAsText 0x0060DA80 reads lo x/y/z +0..+8 and hi x/y/z +0xC..+0x14.
// Special members (exports): default constructor folds onto 0x0047A6A9 and the
// destructor onto the bare ret 0x000B3FD0 (user-declared empty inlines); the copy
// constructor is the COMDAT fold 0x0009AC04 that some callers reach by call
// (WaterTracksObj::Get_Obj_Space_Bounding_Box, W3DShadowTexture::setBoundingBox):
// an inline definition. width/height/depth/zero/isInRegionNoZ/isInRegionWithZ are
// exported (0x00004DDA..0x00004E46) with no direct callers: inline. expandBy
// (0x00006495) lives in the base-type library run: out of line.
#pragma once
#ifndef BFME2_REGION3D_H
#define BFME2_REGION3D_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"

struct Region3D
{
	Coord3D lo;		// 0x0
	Coord3D hi;		// 0xC

	Region3D() {}
	Region3D(const Region3D &that) : lo(that.lo), hi(that.hi) {}
	~Region3D() {}

	Real width() const { return hi.x - lo.x; }
	Real height() const { return hi.y - lo.y; }
	Real depth() const { return hi.z - lo.z; }

	void zero() { lo.zero(); hi.zero(); }

	Bool isInRegionNoZ(const Coord3D *query) const
	{
		return (lo.x < query->x) && (query->x < hi.x)
			&& (lo.y < query->y) && (query->y < hi.y);
	}
	Bool isInRegionWithZ(const Coord3D *query) const
	{
		return (lo.x < query->x) && (query->x < hi.x)
			&& (lo.y < query->y) && (query->y < hi.y)
			&& (lo.z < query->z) && (query->z < hi.z);
	}

	void expandBy(const Coord3D &point);
};
// SIZE 0x18
typedef char Region3DSize[(sizeof(Region3D) == 0x18) ? 1 : -1];

#endif
