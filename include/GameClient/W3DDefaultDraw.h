// W3DDefaultDraw.h: a draw module with no state of its own.
// Target facts (retail game.dat 1.06):
// - Constructor 0x000B21B3 calls DrawModule's (0x000B19A1) and stores table 0x007C98B0 at +0; nothing else.
//   The factory news 0xC bytes.
// - Table 0x007C98B0 (62 slots) overrides only slot 2 (GetSnapshotName 0x000B2214, `mov eax,"W3DDefaultDraw"; ret`),
//   3 (xfer 0x000B221A: Version1, then DrawModule::xfer, the body W3DPropDraw's slot 3 holds), 4 (the
//   module key 0x000B21CF) and the seven pure slots 11-14, 35, 38 and 39 with empty bodies (the folded
//   `ret 4`, `ret` and `ret 0Ch`); slot 1 is the same 0x0058B03E as W3DPropDraw's. SIZE 0xC.
#pragma once
#ifndef BFME2_W3DDEFAULTDRAW_H
#define BFME2_W3DDEFAULTDRAW_H

#include "Common/GameType.h"
#include "GameClient/DrawModule.h"

class Matrix3D;
class Xfer;
class Coord3D;

class W3DDefaultDraw : public DrawModule
{
public:
	W3DDefaultDraw(Thing *thing, const ModuleData *moduleData);				// 0x000B21B3

	virtual NameKeyType getModuleNameKey() const;							// 0x000B21CF
	virtual void doDrawModule(const Matrix3D *transformMtx) {}				// slot 11, folded `ret 4`
	virtual void setShadowsEnabled(Bool enable) {}							// slot 12, folded `ret 4`
	virtual void releaseShadows() {}										// slot 13, folded `ret`
	virtual void allocateShadows() {}										// slot 14, folded `ret`
	virtual void setFullyObscuredByShroud(Bool fullyObscured) {}			// slot 35, folded `ret 4`
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle) {}	// slot 38, folded `ret 0Ch`
	virtual void reactToGeometryChange() {}									// slot 39, folded `ret`

protected:
	virtual void loadPostProcess() { DrawModule::loadPostProcess(); }	// slot 1: retail table 0x007C98B0 holds 0x0058B03E (`jmp 0x000B3FD0`, DrawModule's empty body), ICF-folded with the module bases' (h05)
	virtual const char *GetSnapshotName() const;							// 0x000B2214
public:
	const char *name() const;						// 0x000B2214 (non-virtual, the literal)
protected:
	virtual void xfer(Xfer *xfer);											// 0x000B221A
};
// SIZE 0xC
typedef char W3DDefaultDraw_size[(sizeof(W3DDefaultDraw) == 0xC) ? 1 : -1];

#endif
