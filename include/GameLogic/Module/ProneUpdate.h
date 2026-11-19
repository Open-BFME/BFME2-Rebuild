// ProneUpdate.h: the update that keeps an object prone for a while after it
// takes damage. Target facts: UpdateModule with one member, m_proneFrames at
// +0x20 (ctor 0x0049FF06 stores it); the pool key is 0x0049FE9A and update
// 0x0049FFA0 is slot 0 of the update interface 0x00C51910.
#pragma once
#ifndef BFME2_PRONEUPDATE_H
#define BFME2_PRONEUPDATE_H

#include "GameLogic/Module/UpdateModule.h"

class ProneUpdateModuleData;
class DamageInfo;

class ProneUpdate : public UpdateModule
{
public:
	ProneUpdate(Thing *thing, const ModuleData *moduleData);

	virtual UpdateSleepTime update();
	virtual NameKeyType getModuleNameKey() const;

	void goProne(const DamageInfo *damageInfo);

protected:
	virtual const char *GetSnapshotName() const;	// 0x0049FE94 (slot 2 of the ProneUpdate table; was a non-virtual name())
	const ProneUpdateModuleData *getProneUpdateModuleData() const
	{
		return (const ProneUpdateModuleData *)getModuleData();
	}

	void startProneEffects();
	void stopProneEffects();

	Int m_proneFrames;						// 0x20
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x24

#endif
