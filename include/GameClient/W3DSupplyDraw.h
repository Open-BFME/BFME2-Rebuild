// W3DSupplyDraw.h: the model draw that shows a carried supply load (W3DScriptedModelDraw subclass).
// Target facts: the constructor 0x000CAE43 stores the primary table 0x007CBFC0 (68 slots), its own
// ObjectDrawInterface table 0x007CBEF8 and the base's one-slot table 0x007CA08C. Against the base's
// tables it overrides primary slots 0-4 (slot 1 is the loadPostProcess forwarder 0x000CA132) and
// ObjectDrawInterface slot 8, updateDrawModuleSupplyStatus (0x000CAFA6). xfer 0x000CAEDF writes two
// Ints at +0x2E8 and +0x2EC before the base's; the members wait for the base class's layout.
#pragma once
#ifndef BFME2_W3DSUPPLYDRAW_H
#define BFME2_W3DSUPPLYDRAW_H

#include "GameClient/W3DScriptedModelDraw.h"

class W3DSupplyDraw : public W3DScriptedModelDraw
{
public:
	W3DSupplyDraw(Thing *thing, const ModuleData *moduleData);				// 0x000CAE43
	virtual NameKeyType getModuleNameKey() const;							// 4: 0x000CAE7B
	virtual void updateDrawModuleSupplyStatus(Int maxSupply, Int currentSupply);	// ObjectDrawInterface 8: 0x000CAFA6

protected:
	virtual ~W3DSupplyDraw();												// 0: scalar deleting 0x000CAF6E
	virtual void loadPostProcess();											// 1: 0x000CA132
	virtual const char *GetSnapshotName() const;							// 2: 0x000CAEC0
	virtual void xfer(Xfer *xfer);											// 3: 0x000CAEDF
};

#endif
