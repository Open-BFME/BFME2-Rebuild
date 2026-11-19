// GrantUpgradeCreate.h: the create module that grants an upgrade when the object
// is created or finished building. Target facts: CreateModule (size 0x18) with
// onCreate 0x004B901D and onBuildComplete 0x004B9093 as slots 0 and 1 of the
// CreateModuleInterface table 0x00859428; ctor 0x004B8F62 and pool key
// 0x004B8FAB close the class.
#pragma once
#ifndef BFME2_GRANTUPGRADECREATE_H
#define BFME2_GRANTUPGRADECREATE_H

#include "GameLogic/Module/CreateModule.h"

class Xfer;

class GrantUpgradeCreate : public CreateModule
{
public:
	GrantUpgradeCreate(Thing *thing, const ModuleData *moduleData);

	virtual void onCreate();
	virtual void onBuildComplete();
	virtual NameKeyType getModuleNameKey() const;
protected:
	virtual const char *GetSnapshotName() const;	// 0x004B8F8C, primary slot 2; retail's table has this class's own (h05, gen3 vtable-content list)
	virtual void xfer(Xfer *xfer);	// primary slot 3: retail 0x004851E3, Version1 then CreateModule::xfer (the fold PreorderCreate and LockWeaponCreate share; h05 r9)
protected:
	virtual void loadPostProcess() { CreateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to CreateModule's, ICF-folded across classes (h05)
};
// SIZE 0x18

#endif
