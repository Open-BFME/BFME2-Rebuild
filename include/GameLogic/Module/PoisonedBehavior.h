// PoisonedBehavior.h: the update/damage module that keeps a poisoned object
// losing health on an interval. Target facts: UpdateModule (size 0x20) plus
// DamageModuleInterface at +0x20, so the poison fields start at +0x24; the
// primary table is 0x00C498BC, the update interface 0x00C497F4 and the damage
// interface 0x00C497E8. Members come from the ctor 0x00482F7D and xfer
// 0x004830A9 (uint damage frame, uint overall stop frame, float amount, int
// death type).
#pragma once
#ifndef BFME2_POISONEDBEHAVIOR_H
#define BFME2_POISONEDBEHAVIOR_H

#include "GameLogic/Module/DamageModule.h"
#include "GameLogic/Module/UpdateModule.h"

class PoisonedBehaviorModuleData;

class PoisonedBehavior : public UpdateModule, public DamageModuleInterface
{
public:
	PoisonedBehavior(Thing *thing, const ModuleData *moduleData);

	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();
	virtual DisabledMaskType getDisabledTypesToProcess() const;	// override: update slot 1 at +0x10 (retail 0x004DF396, returns DISABLEDMASK_ALL)
	virtual void onDamage(DamageInfo *damageInfo);
	virtual void onHealing(DamageInfo *damageInfo);
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo,
		BodyDamageType oldState, BodyDamageType newState) {}

	void stopPoisonedEffects();			// 0x00483083 (Zero Hour's name)

protected:
	virtual ~PoisonedBehavior();
	virtual void loadPostProcess();
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	void startPoisonedEffects(const DamageInfo *damageInfo);
	UpdateSleepTime calcSleepTime();

	const PoisonedBehaviorModuleData *getPoisonedBehaviorModuleData() const
	{
		return (const PoisonedBehaviorModuleData *)getModuleData();
	}

	UnsignedInt m_poisonDamageFrame;		// 0x24
	UnsignedInt m_poisonOverallStopFrame;	// 0x28
	Real m_poisonDamageAmount;				// 0x2C
	Int m_deathType;						// 0x30
public:
	virtual DamageModuleInterface *getDamage() { return this; }	// BehaviorModuleInterface slot 4: retail's is the shared folded getter 0x004543A9 returning the +0x20 sub-object (h05, gen3 vtable-content list)
};
// SIZE 0x34

#endif
