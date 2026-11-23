// BaseHeightMapRenderObjClass.h: the terrain render object that W3DTerrainLogic forwards its height, cliff and
// line-of-sight queries to (TheTerrainRenderObject, VA 0x00DE1EAC). Only the members W3DTerrainLogic calls are
// declared; the layout is not recovered (a generated vtable draft lives in Draft/BaseHeightMapRenderObjClass.h and
// is not included anywhere). Target facts: each W3DTerrainLogic wrapper loads the pointer and calls or tail-jumps to
// the member named in its comment.
#pragma once
#ifndef BFME2_BASEHEIGHTMAPRENDEROBJCLASS_H
#define BFME2_BASEHEIGHTMAPRENDEROBJCLASS_H

#include "Common/GameType.h"
#include "WW3D2/RenderObjClass.h"

class Coord3D;
class W3DTerrainLogic;

// The retired draft's table named RefCountClass::Delete_This in slot 0, RenderObjClass::Get_Name in slot 6 and
// BaseHeightMapRenderObjClass::Cast_Ray 0x0006BF62 in slot 60, which is RenderObjClass's own Cast_Ray slot.
class BaseHeightMapRenderObjClass : public RenderObjClass
{
public:
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);						// 60: 0x0006BF62
	Bool isClearLineOfSight(const Coord3D &start, const Coord3D &end) const;	// 0x00066AD1
	Bool isCliffCell(Real x, Real y);											// 0x000671B2
	UnsignedByte rva0006B187(Real x, Real y);									// 0x0006B187
	UnsignedByte rva00067225(Real x, Real y);									// 0x00067225
	Bool rva00067298(Real x, Real y);											// 0x00067298
	UnsignedByte rva0006730B(Real x, Real y);									// 0x0006730B
	void loadRoadsAndBridges(W3DTerrainLogic *terrain, Bool saveGame);			// 0x00067800
};

extern BaseHeightMapRenderObjClass *TheTerrainRenderObject;	// 0x00DE1EAC

#endif
