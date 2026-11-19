// W3DQuadrupedDraw.h: the four-legged model draw (W3DScriptedModelDraw subclass).
// Target facts: the constructor 0x000CA0A4 stores the primary table 0x007CBC40 (68 slots), its own
// ObjectDrawInterface table 0x007CBB78 and the base's one-slot table 0x007CA08C. Against the base's
// tables it overrides primary slots 0-4 (slot 1 is the loadPostProcess forwarder 0x000CA132) and
// ObjectDrawInterface slot 47 (0x000CA352). xfer 0x000CA137 adds only its version before the base's.
// Data members are not recovered yet.
#pragma once
#ifndef BFME2_W3DQUADRUPEDDRAW_H
#define BFME2_W3DQUADRUPEDDRAW_H

#include "GameClient/W3DScriptedModelDraw.h"

class W3DQuadrupedDraw : public W3DScriptedModelDraw
{
public:
	W3DQuadrupedDraw(Thing *thing, const ModuleData *moduleData);			// 0x000CA0A4
	virtual NameKeyType getModuleNameKey() const;							// 4: 0x000CA0CE
	virtual Bool vslot47(Int *a, void *b);									// ObjectDrawInterface 47: 0x000CA352

protected:
	virtual ~W3DQuadrupedDraw();											// 0: scalar deleting 0x000CA1B7
	virtual void loadPostProcess();											// 1: 0x000CA132
	virtual const char *GetSnapshotName() const;							// 2: 0x000CA113
	virtual void xfer(Xfer *xfer);											// 3: 0x000CA137
};

#endif
