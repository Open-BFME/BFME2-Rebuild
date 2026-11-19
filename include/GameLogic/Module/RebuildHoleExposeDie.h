// RebuildHoleExposeDie.h: the die module that leaves a rebuild hole where the
// object stood. Target facts: DieModule (BehaviorModule + DieModuleInterface at
// +0x10, size 0x14); the primary table is 0x00C4AE54, the ctor 0x00486768
// calls DieModule's and adds no member, the pool key is 0x00486792 and onDie
// 0x004867FE is slot 0 of the die interface.
#pragma once
#ifndef BFME2_REBUILDHOLEEXPOSEDIE_H
#define BFME2_REBUILDHOLEEXPOSEDIE_H

#include "GameLogic/Module/DieModule.h"

class RebuildHoleExposeDie : public DieModule
{
public:
	RebuildHoleExposeDie(Thing *thing, const ModuleData *moduleData);

	virtual void onDie(const DamageInfo *damageInfo);
	virtual NameKeyType getModuleNameKey() const;

	const char *name() const;

protected:
	virtual ~RebuildHoleExposeDie();
	virtual void xfer(Xfer *xfer);
protected:
	virtual const char *GetSnapshotName() const;	// 0x004867D7, primary slot 2; retail's table has this class's own (h05, gen3 vtable-content list)
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to DieModule's, ICF-folded across classes (h05)
};
// SIZE 0x14

#endif
