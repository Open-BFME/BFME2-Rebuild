// UpgradeDie.h: the die module that removes an upgrade from its producer.
// Target facts: a DieModule that adds no member; ctor 0x00486BBA calls
// DieModule's and stores 0x00C4AEBC at +0, 0x00C4A650 at +0xC and 0x00C4AEB8
// at +0x10 (its onDie 0x00486C00).
#pragma once
#ifndef BFME2_UPGRADEDIE_H
#define BFME2_UPGRADEDIE_H

#include "GameLogic/Module/DieModule.h"

class Xfer;
class UpgradeDie : public DieModule
{
public:
	UpgradeDie(Thing *thing, const ModuleData *moduleData);				// 0x00486BBA

	virtual NameKeyType getModuleNameKey() const;						// 0x00486B75
	virtual const char *GetSnapshotName() const;
	virtual void onDie(const DamageInfo *damageInfo);					// 0x00486C00
protected:
	virtual void xfer(Xfer *xfer);	// primary slot 3: retail's body is folded at 0x0045CE8C; retail's table has this class's own (h05, gen3 vtable-content list)
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to DieModule's, ICF-folded across classes (h05)
};

#endif
