// W3DPropDraw.h: the prop draw module (hands its model to the terrain once, when the drawable
// first has a non-origin position).
// Target facts (retail game.dat 1.06):
// - Constructor 0x000CEEC0 calls DrawModule's (0x000B19A1), stores table 0x007CD328 at +0 and
//   clears the byte at +0x0C. Table 0x007CD328 has 62 slots; it overrides 2 (GetSnapshotName
//   0x000CEEE0), 3 (xfer 0x000B221A: Version1, then DrawModule::xfer, ICF-folded across the
//   empty draw modules), 4 (the module key 0x000CEEE6), 11, 12, 13, 14, 35 and 39 (empty
//   bodies) and 38 (reactToTransformChange 0x000CEF9F). SIZE 0x10.
// - reactToTransformChange tests +0x0C, asks the drawable for its position and, when x or y is
//   non-zero, sets +0x0C and passes the drawable's id, position, orientation (+0x44 of the
//   drawable), scale (+0x200) and the module data's name and fog flag to the terrain's addProp
//   0x0006831C.
// Donor facts: Zero Hour's W3DPropDraw has the same shape (m_propCreated, W3DPropDrawModuleData
//   m_modelName and m_shadowType); the BFME 2 data has a DistanceFog flag instead of a shadow.
#pragma once
#ifndef BFME2_W3DPROPDRAW_H
#define BFME2_W3DPROPDRAW_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameClient/DrawModule.h"

class Matrix3D;
class Xfer;
class Coord3D;

class W3DPropDraw : public DrawModule
{
public:
	W3DPropDraw(Thing *thing, const ModuleData *moduleData);				// 0x000CEEC0

	virtual NameKeyType getModuleNameKey() const;							// 0x000CEEE6 (map: the static placeholder rva000CEEE6)
	virtual void doDrawModule(const Matrix3D *transformMtx) {}				// slot 11, folded `ret 4`
	virtual void setShadowsEnabled(Bool enable) {}							// slot 12, folded `ret 4`
	virtual void releaseShadows() {}										// slot 13, folded `ret`
	virtual void allocateShadows() {}										// slot 14, folded `ret`
	virtual void setFullyObscuredByShroud(Bool fullyObscured) {}			// slot 35, folded `ret 4`
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle);	// 0x000CEF9F
	virtual void reactToGeometryChange() {}									// slot 39, folded `ret`

protected:
	virtual void loadPostProcess() { DrawModule::loadPostProcess(); }	// slot 1: retail table 0x007CD328 holds 0x0058B03E (`jmp 0x000B3FD0`, DrawModule's empty body), ICF-folded with the module bases' (h05)
	virtual const char *GetSnapshotName() const;							// 0x000CEEE0
	virtual void xfer(Xfer *xfer);											// 0x000B221A

public:
	Bool m_propAdded;														// 0x0C
};
// SIZE 0x10
typedef char W3DPropDraw_size[(sizeof(W3DPropDraw) == 0x10) ? 1 : -1];
typedef char W3DPropDraw_off_m_propAdded[(offsetof(W3DPropDraw, m_propAdded) == 0xC) ? 1 : -1];

#endif
