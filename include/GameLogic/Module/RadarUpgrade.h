// RadarUpgrade.h: the upgrade that gives its object's controlling player a radar
// and takes it away again on delete and on capture. SIZE 0x1C: it adds no
// members to UpgradeModule.
// Target facts: the constructor 0x004B478E calls UpgradeModule and then installs
// its four vtables; the destructor 0x004B46F9 installs them again and tail-jumps
// to ~UpgradeModule. onDelete 0x004B47DB and onCapture 0x004B4828 read the
// module data's DisableProof flag at +0x118 and Object's disabled mask at +0x1C8.
// upgradeImplementation 0x004B488F calls RadarUpdate::extendRadar, which has no
// real header yet, so it is declared but not written here.
#pragma once
#ifndef BFME2_RADARUPGRADE_H
#define BFME2_RADARUPGRADE_H

#include "GameLogic/Module/UpgradeModule.h"

class Player;

class Xfer;
class RadarUpgrade : public UpgradeModule
{
public:
	RadarUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual ~RadarUpgrade();											// 0x004B46F9

	virtual NameKeyType getModuleNameKey() const;						// 0x004B4727
	virtual void onDelete();											// 0x004B47DB
	virtual void onCapture(Player *oldOwner, Player *newOwner);			// 0x004B4828

protected:
	virtual const char *GetSnapshotName() const;						// 0x004B4719
	virtual void upgradeRemovalImplementation();						// 0x004B471F
	virtual void upgradeImplementation();								// 0x004B488F
protected:
	virtual void xfer(Xfer *xfer);	// primary slot 3: retail's body is folded with ArmorUpgrade::xfer 0x004B8710; retail's table has this class's own (h05, gen3 vtable-content list)
protected:
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpgradeModule's, ICF-folded across classes (h05)
};

#endif
