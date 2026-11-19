// InstantDeathBehavior.h: the InstantDeathBehavior die module. SIZE 0x14.
// Target facts: the constructor 0x0045CF55 calls the DieModule constructor (0x0045CEBD) on
// this object and then installs the class's own tables; the class adds no member of its own,
// so its size is DieModule's 0x14. onDie (0x0045CF9B) overrides DieModuleInterface's slot.
// Its module data is InstantDeathBehaviorModuleData (see that header).
#pragma once
#ifndef BFME2_INSTANT_DEATH_BEHAVIOR_H
#define BFME2_INSTANT_DEATH_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DieModule.h"

class Thing;
class ModuleData;
class Xfer;
class DamageInfo;

// DieModule ends at 0x14; this class adds no member.
typedef char InstantDeathBehavior_base_size[(sizeof(DieModule) == 0x14) ? 1 : -1];

class InstantDeathBehavior : public DieModule
{
public:
	InstantDeathBehavior(Thing *thing, const ModuleData *moduleData);	// 0x0045CF55
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual void onDie(const DamageInfo *damageInfo);					// 0x0045CF9B

protected:
	virtual void xfer(Xfer *xfer);
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to DieModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char InstantDeathBehavior_size[(sizeof(InstantDeathBehavior) == 0x14) ? 1 : -1];

#endif
