// CreateObjectDie.h: the die module that spawns an object where this one died.
// Target facts: DieModule (BehaviorModule + DieModuleInterface at +0x10, size
// 0x14) with onDie at slot 0 of its die table 0x00C4A7DC (0x00485C86); its
// Snapshot class-name getter is 0x00485956 ("CreateObjectDie") and the pool key
// is the static 0x004859EA.
#pragma once
#ifndef BFME2_CREATEOBJECTDIE_H
#define BFME2_CREATEOBJECTDIE_H

#include "GameLogic/Module/DieModule.h"

class CreateObjectDie : public DieModule
{
public:
	CreateObjectDie(Thing *thing, const ModuleData *moduleData);

	virtual void onDie(const DamageInfo *damageInfo);
	virtual const char *GetSnapshotName() const;   // 0x00485956 (literal class-name getter)

	static NameKeyType rva0004859EA();

protected:
	virtual ~CreateObjectDie();
};
// SIZE 0x14

#endif
