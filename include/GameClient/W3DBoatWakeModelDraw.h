// W3DBoatWakeModelDraw.h: the boat wake draw module (DrawModule subclass).
// Target facts (retail game.dat 1.06):
// - Constructor 0x000D0B34 calls DrawModule's and starts its own fields at +0x0C; destructor
//   0x000D0B69; table 0x007CDD70 (62 slots). Against DrawModule's table 0x007C9690 it overrides 0-4
//   (GetSnapshotName 0x000D0B63, xfer 0x000D0BC3, the module key 0x000D0C93; slot 1 is the module
//   bases' loadPostProcess fold 0x0058B03E), 11 (doDrawModule 0x000D0D24), 12, 13, 14, 35 and 39
//   (empty bodies), 38 (0x000D1056, three arguments) and 49 (getRenderObject: a member load that
//   folds with GameClient's 0x0057E556). Data members are not recovered yet.
#pragma once
#ifndef BFME2_W3DBOATWAKEMODELDRAW_H
#define BFME2_W3DBOATWAKEMODELDRAW_H

#include "GameClient/DrawModule.h"

class W3DBoatWakeModelDraw : public DrawModule
{
public:
	W3DBoatWakeModelDraw(Thing *thing, const ModuleData *moduleData);		// 0x000D0B34
	virtual NameKeyType getModuleNameKey() const;							// 4: 0x000D0C93
	virtual void doDrawModule(const Matrix3D *transformMtx);				// 11: 0x000D0D24
	virtual void setShadowsEnabled(Bool enable) {}							// 12: folded `ret 4`
	virtual void releaseShadows() {}										// 13: folded `ret`
	virtual void allocateShadows() {}										// 14: folded `ret`
	virtual void setFullyObscuredByShroud(Bool fullyObscured) {}			// 35: folded `ret 4`
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle);	// 38: 0x000D1056
	virtual void reactToGeometryChange() {}									// 39: folded `ret`
	virtual RenderObjClass *getRenderObject();								// 49: 0x0057E556 (fold)

protected:
	virtual ~W3DBoatWakeModelDraw();										// 0x000D0B69
	virtual void loadPostProcess() { DrawModule::loadPostProcess(); }	// 1: the module bases' fold 0x0058B03E
	virtual const char *GetSnapshotName() const;							// 2: 0x000D0B63
	virtual void xfer(Xfer *xfer);											// 3: 0x000D0BC3
};

#endif
