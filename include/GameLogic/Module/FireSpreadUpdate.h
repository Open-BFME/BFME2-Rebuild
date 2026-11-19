// FireSpreadUpdate.h: the module that propagates fire to neighbours. SIZE 0x20: the module adds
// no member of its own over UpdateModule.
// Target facts: the constructor 0x0048B769 calls the UpdateModule constructor 0x00253390 and then
// sets the wake frame to forever; startFireSpreading 0x0048B938 tests the object's status bit 10
// and, when it is set, wakes the object after calcNextSpreadDelay 0x0048B7D9 (a random delay from
// the module data's min and max, never below 1). update 0x0048B7FD is not written yet.
#pragma once
#ifndef BFME2_FIRESPREADUPDATE_H
#define BFME2_FIRESPREADUPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;

class FireSpreadUpdate : public UpdateModule
{
public:
	FireSpreadUpdate(Thing *thing, const ModuleData *moduleData);	// 0x0048B769
	virtual NameKeyType getModuleNameKey() const;					// slot 4, 0x0048B690 (the map spells it as a static: rva00048B690)
	virtual UpdateSleepTime update();								// 0x0048B7FD

	void startFireSpreading();										// 0x0048B938
	UnsignedInt calcNextSpreadDelay();								// 0x0048B7D9

protected:
	virtual const char *GetSnapshotName() const;					// 0x0048B68A
	virtual void xfer(Xfer *xfer);									// slot 3, 0x004DF81B: Version1, then UpdateModule's (f08)
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x20
typedef char FireSpreadUpdate_size[(sizeof(FireSpreadUpdate) == 0x20) ? 1 : -1];

#endif
