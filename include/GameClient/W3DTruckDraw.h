// W3DTruckDraw.h: the wheeled-vehicle model draw (W3DScriptedModelDraw subclass).
// Target facts: the constructor 0x000CB3FB calls W3DScriptedModelDraw's 0x000C0DD8 and stores the
// primary table 0x007CC650 (68 slots) and the base's two secondary tables. Against the base's table
// 0x007CA090 it overrides slots 0-4, 11 (doDrawModule 0x000CC80E), 16 (setHidden 0x000CC781),
// 35 (setFullyObscuredByShroud 0x000CDF0A) and 63 (onRenderObjRecreated 0x000CC7A1). The map spells
// two of these bodies W3DTankTruckDraw (Zero Hour's class); in retail they are only in this table.
// Data members are not recovered yet.
#pragma once
#ifndef BFME2_W3DTRUCKDRAW_H
#define BFME2_W3DTRUCKDRAW_H

#include "GameClient/W3DScriptedModelDraw.h"

class W3DTruckDraw : public W3DScriptedModelDraw
{
public:
	W3DTruckDraw(Thing *thing, const ModuleData *moduleData);				// 0x000CB3FB
	virtual NameKeyType getModuleNameKey() const;							// 4: 0x000CB578
	virtual void doDrawModule(const Matrix3D *transformMtx);				// 11: 0x000CC80E
	virtual void setHidden(Bool hidden);									// 16: 0x000CC781
	virtual void setFullyObscuredByShroud(Bool fullyObscured);				// 35: 0x000CDF0A
	virtual void onRenderObjRecreated();									// 63: 0x000CC7A1

protected:
	virtual ~W3DTruckDraw();												// 0: scalar deleting 0x000CDF34
	virtual void loadPostProcess();											// 1: 0x000CDE47
	virtual const char *GetSnapshotName() const;							// 2: 0x000CB5BD
	virtual void xfer(Xfer *xfer);											// 3: 0x000CB14E
};

#endif
