// W3DHordeModelDraw.h: the horde model draw (W3DScriptedModelDraw subclass).
// Target facts: the constructor 0x00078A82 calls W3DScriptedModelDraw's 0x000C0DD8 and stores the
// primary table 0x007C68F0 (68 slots) and the base's two secondary tables. Against the base's table
// 0x007CA090 it overrides slots 0-4, 6 (onDrawableBoundToObject 0x0007A7DD), 64 (setModelState
// 0x0007A847) and the two slots 66 and 67, whose bodies (0x000791DD, 0x000791E5) forward to an object
// held at +0x04 of the module. Data members are not recovered yet.
#pragma once
#ifndef BFME2_W3DHORDEMODELDRAW_H
#define BFME2_W3DHORDEMODELDRAW_H

#include "GameClient/W3DScriptedModelDraw.h"

class W3DHordeModelDraw : public W3DScriptedModelDraw
{
public:
	W3DHordeModelDraw(Thing *thing, const ModuleData *moduleData);			// 0x00078A82
	virtual NameKeyType getModuleNameKey() const;							// 4: 0x00078B38
	virtual void onDrawableBoundToObject();									// 6: 0x0007A7DD

protected:
	virtual ~W3DHordeModelDraw();											// 0: scalar deleting 0x000791C1
	virtual void loadPostProcess();											// 1: 0x000CA132
	virtual const char *GetSnapshotName() const;							// 2: 0x00078B7D
	virtual void xfer(Xfer *xfer);											// 3: 0x00078BBB
	virtual Bool setModelState(const ModelConditionInfo *newState, Bool arg2, void *arg3);	// 64: 0x0007A847
	virtual Int slot66();													// 66: 0x000791DD
	virtual Int slot67(Int count);											// 67: 0x000791E5
};

#endif
