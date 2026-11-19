// ExperienceScalarUpgrade.h: the ExperienceScalarUpgrade upgrade module. SIZE 0x1C.
// Target facts: the constructor 0x004B61C6 calls UpgradeModule's constructor (0x00460AEC) and then
// installs the class's tables; the class adds no member of its own. upgradeImplementation (0x004B6213)
// adds the module data's AddXPScalar to the owning object's experience tracker scalar, and
// upgradeRemovalImplementation (0x004B6236) subtracts it again.
#pragma once
#ifndef BFME2_EXPERIENCE_SCALAR_UPGRADE_H
#define BFME2_EXPERIENCE_SCALAR_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpgradeModule ends at 0x1C; this class adds no member.
typedef char ExperienceScalarUpgrade_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class ExperienceScalarUpgrade : public UpgradeModule
{
public:
	ExperienceScalarUpgrade(Thing *thing, const ModuleData *moduleData);	// 0x004B61C6
	virtual ~ExperienceScalarUpgrade();									// 0x004B611C
	virtual NameKeyType getModuleNameKey() const;						// 0x004B6142

protected:
	virtual const char *GetSnapshotName() const;						// 0x004B613C
	virtual void xfer(Xfer *xfer);
	virtual void upgradeImplementation();								// 0x004B6213
	virtual void upgradeRemovalImplementation();						// 0x004B6236
protected:
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpgradeModule's, ICF-folded across classes (h05)
};
// SIZE 0x1C
typedef char ExperienceScalarUpgrade_size[(sizeof(ExperienceScalarUpgrade) == 0x1C) ? 1 : -1];

#endif
