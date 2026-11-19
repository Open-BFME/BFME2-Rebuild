// FlammableUpdate.h: manages the Aflame and Burned statuses and their effects.
// Target facts (old repo reference/shims/bfme2_flammableupdate/GameLogic/Module/
// FlammableUpdate.h): the instance factory 0x0024D030 news 0x50 bytes and calls
// the 0x0048C64F constructor. UpdateModule's base ends at +0x20, the
// DamageModuleInterface table sits at +0x20 and the members follow from +0x24;
// 0x0048C582 (xfer) reads status +0x24, the three end frames +0x28/+0x2C/+0x30,
// the audio handle +0x34, the limit +0x38, the last dealt +0x3C, the fleeing
// flag +0x40, the special-condition frame +0x44, the igniter id +0x48 and the
// tail flag +0x4C.
#pragma once
#ifndef BFME2_FLAMMABLEUPDATE_H
#define BFME2_FLAMMABLEUPDATE_H

#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DamageModule.h"
#include "GameLogic/Module/FlammableUpdateModuleData.h"
#include "Common/GameAudio.h"

class DamageInfo;

//-------------------------------------------------------------------------------------------------
enum FlammabilityStatusType
{
	// These show the state I last noticed my object was in.
	FS_NORMAL = 0,
	FS_AFLAME,
	FS_BURNED,

	FS_NORMAL_COUNT	// keep last
};

//-------------------------------------------------------------------------------------------------
class FlammableUpdate : public UpdateModule, public DamageModuleInterface
{
public:
	FlammableUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~FlammableUpdate();											// slot 0

	virtual NameKeyType getModuleNameKey() const;						// Module slot 4

	void tryToIgnite();		///< FlammableDamage uses this. It is up to me to decide if I am burnable
	Bool wouldIgnite();		///< Since we need to cheat sometimes and light something directly, ask if this would light
	void doAflameDamage();
	void rva0048C7BF();		///< starts the burning sound
	void rva0048C8F9();		///< ends the burn

	virtual UpdateSleepTime update();									// UpdateModuleInterface slot 0

	virtual DamageModuleInterface *getDamage() { return this; }			// BehaviorModuleInterface slot 4
	virtual void onDamage(DamageInfo *damageInfo);
	virtual void onHealing(DamageInfo *damageInfo);
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo,
		BodyDamageType oldState, BodyDamageType newState) {}

protected:
	virtual const char *GetSnapshotName() const;						// Snapshot slot 2
	virtual void xfer(Xfer *xfer);										// Snapshot slot 3
	virtual void loadPostProcess();

	UpdateSleepTime calcSleepTime();
	void stopBurningSound();
	const FlammableUpdateModuleData *getFlammableUpdateModuleData() const
	{ return (const FlammableUpdateModuleData *)getModuleData(); }

	FlammabilityStatusType m_status;			// 0x24
	UnsignedInt m_aflameEndFrame;				// 0x28
	UnsignedInt m_burnedEndFrame;				// 0x2C
	UnsignedInt m_damageEndFrame;				// 0x30
	AudioHandle m_audioHandle;					// 0x34
	Real m_flameDamageLimit;					// 0x38
	UnsignedInt m_lastFlameDamageDealt;			// 0x3C
	Bool m_fleeing;								// 0x40
	UnsignedInt m_specialConditionEndFrame;		// 0x44
	ObjectID m_lastIgniterID;					// 0x48
	Bool m_flag4C;								// 0x4C
};
// SIZE 0x50

#endif
