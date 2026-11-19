// W3DSailModelDraw.h: the model draw that turns a ship's sails (W3DScriptedModelDraw subclass).
// Target facts: the constructor 0x000D078F calls W3DScriptedModelDraw's 0x000C0DD8 and stores the
// primary table 0x007CDC60 (68 slots) and the base's two secondary tables. Against the base's table
// 0x007CA090 it overrides slots 0-4 and slot 65 (adjustTransformMtx, 0x000D08C3); slot 1 is the
// loadPostProcess forwarder 0x000CA132 it shares with W3DHordeModelDraw and W3DSupplyDraw.
// Data members are not recovered yet.
#pragma once
#ifndef BFME2_W3DSAILMODELDRAW_H
#define BFME2_W3DSAILMODELDRAW_H

#include "GameClient/W3DScriptedModelDraw.h"

class W3DSailModelDraw : public W3DScriptedModelDraw
{
public:
	W3DSailModelDraw(Thing *thing, const ModuleData *moduleData);			// 0x000D078F
	virtual NameKeyType getModuleNameKey() const;							// 4: 0x000D07CB

protected:
	virtual ~W3DSailModelDraw();											// 0: scalar deleting 0x000D08A2
	virtual void loadPostProcess();											// 1: 0x000CA132
	virtual const char *GetSnapshotName() const;							// 2: 0x000D0810
	virtual void xfer(Xfer *xfer);											// 3: 0x000D0816
	virtual void adjustTransformMtx(Matrix3D &mtx) const;					// 65: 0x000D08C3
};

#endif
