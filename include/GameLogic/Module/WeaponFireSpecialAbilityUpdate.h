// WeaponFireSpecialAbilityUpdate.h: a special ability that fires a dedicated weapon.
// Target facts (old repo Code/GameEngine/Source/GameLogic/Object/Update/WeaponFireSpecialAbilityUpdate.cpp):
//  - module data extends SpecialAbilityUpdateModuleData (0xC8): SpecialWeapon name
//    +0xC8, WhichSpecialWeapon +0xCC, SkipContinue +0xD0, BusyForDuration +0xD4,
//    NeedLivingTargets +0xD8, PlayWeaponPreFireFX +0xD9;
//  - the module extends SpecialAbilityUpdate (0x88) with its Weapon at +0x88, built
//    from TheWeaponStore by the constructor (0x00492708) and deleted by the
//    destructor (0x0049256B). Primary vtable 0x00C4E090.
#pragma once
#ifndef BFME2_WEAPONFIRESPECIALABILITYUPDATE_H
#define BFME2_WEAPONFIRESPECIALABILITYUPDATE_H

#include "GameLogic/Module/SpecialAbilityUpdate.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "Common/AsciiString.h"

class Weapon;

class WeaponFireSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	AsciiString m_specialWeaponName;		// 0xC8
	Int m_whichSpecialWeapon;				// 0xCC
	Bool m_skipContinue;					// 0xD0
	Int m_busyForDuration;					// 0xD4
	Bool m_needLivingTargets;				// 0xD8
	Bool m_playWeaponPreFireFX;				// 0xD9
};

class WeaponFireSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	WeaponFireSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~WeaponFireSpecialAbilityUpdate();
	virtual NameKeyType getModuleNameKey() const;
	const char *name() const;
	virtual bool continuePreparation();
	virtual void triggerAbilityEffect();
	virtual void startPacking(bool);
	virtual void startUnpacking();
	virtual UpdateSleepTime update();
	virtual bool vslot08(int value);

	const WeaponFireSpecialAbilityUpdateModuleData *getWeaponFireSpecialAbilityUpdateModuleData() const
	{
		return (const WeaponFireSpecialAbilityUpdateModuleData *)m_moduleData;
	}

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void onExit(bool a, bool b);

	Weapon *m_weapon;		// 0x88
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's 0x00497805 (h05 r13)
};
// SIZE 0x8C

#endif
