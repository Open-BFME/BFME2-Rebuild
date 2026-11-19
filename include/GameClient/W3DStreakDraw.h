// W3DStreakDraw.h: the streak draw module (DrawModule subclass).
// Target facts (retail game.dat 1.06):
// - Constructor 0x000D02A7, destructor 0x000CFFD3, table 0x007CD9B8 (62 slots). Against DrawModule's
//   table 0x007C9690 it overrides 0-4 (GetSnapshotName 0x000D0046, the module key 0x000D004C; slot 1
//   is the module bases' loadPostProcess fold 0x0058B03E, slot 3 the empty draw modules' xfer fold
//   0x000B221A), 11 and 38 (0x000D0461 and 0x000D0459: both only call the streak update 0x000D0091),
//   12, 13, 14, 35 and 39 (empty bodies) and 16 (setHidden 0x000CFF4C, which forwards to the render
//   object at +0x0C). Data members are not recovered yet.
#pragma once
#ifndef BFME2_W3DSTREAKDRAW_H
#define BFME2_W3DSTREAKDRAW_H

#include "GameClient/DrawModule.h"

class W3DStreakDraw : public DrawModule
{
public:
	W3DStreakDraw(Thing *thing, const ModuleData *moduleData);				// 0x000D02A7
	virtual NameKeyType getModuleNameKey() const;							// 4: 0x000D004C
	virtual void doDrawModule(const Matrix3D *transformMtx);				// 11: 0x000D0461
	virtual void setShadowsEnabled(Bool enable) {}							// 12: folded `ret 4`
	virtual void releaseShadows() {}										// 13: folded `ret`
	virtual void allocateShadows() {}										// 14: folded `ret`
	virtual void setHidden(Bool hidden);									// 16: 0x000CFF4C
	virtual void setFullyObscuredByShroud(Bool fullyObscured) {}			// 35: folded `ret 4`
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle);	// 38: 0x000D0459
	virtual void reactToGeometryChange() {}									// 39: folded `ret`

	void rva000D0091();														// 0x000D0091: the streak update slots 11 and 38 call (map placeholder name)

protected:
	virtual ~W3DStreakDraw();												// 0x000CFFD3
	virtual void loadPostProcess() { DrawModule::loadPostProcess(); }	// 1: the module bases' fold 0x0058B03E
	virtual const char *GetSnapshotName() const;							// 2: 0x000D0046
	virtual void xfer(Xfer *xfer) { xfer->Version1(); DrawModule::xfer(xfer); }	// 3: the empty draw modules' fold 0x000B221A
};

#endif
