// W3DTankDraw.h: the tracked-vehicle model draw (W3DScriptedModelDraw subclass).
// Target facts: the constructor 0x000CE960 stores the primary table 0x007CCBE8 (68 slots) and the
// base's two secondary tables. Against the base's table 0x007CA090 it overrides slots 0-4,
// 11 (doDrawModule 0x000CE3E3), 16 (setHidden 0x000CE129), 35 (setFullyObscuredByShroud 0x000CE147)
// and 63 (onRenderObjRecreated, the jump 0x000CE3DE). Data members are not recovered yet.
#pragma once
#ifndef BFME2_W3DTANKDRAW_H
#define BFME2_W3DTANKDRAW_H

#include "GameClient/W3DScriptedModelDraw.h"

class W3DTankDraw : public W3DScriptedModelDraw
{
public:
	W3DTankDraw(Thing *thing, const ModuleData *moduleData);				// 0x000CE960
	virtual NameKeyType getModuleNameKey() const;							// 4: 0x000CE9EE
	virtual void doDrawModule(const Matrix3D *transformMtx);				// 11: 0x000CE3E3
	virtual void setHidden(Bool hidden);									// 16: 0x000CE129
	virtual void setFullyObscuredByShroud(Bool fullyObscured);				// 35: 0x000CE147
	virtual void onRenderObjRecreated();									// 63: 0x000CE3DE, a jump to updateTreadObjects

protected:
	virtual ~W3DTankDraw();													// 0: scalar deleting 0x000CEB2C
	virtual void loadPostProcess();											// 1: 0x000CEA39
	virtual const char *GetSnapshotName() const;							// 2: 0x000CEA33
	virtual void xfer(Xfer *xfer);											// 3: 0x000CE03C
	void updateTreadObjects();												// 0x000CE16A
	void tossEmitters();													// 0x000CE7B4
	void createEmitters();													// 0x000CE837
};

#endif
