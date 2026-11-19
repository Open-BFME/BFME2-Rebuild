// AimWeaponBehavior.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x0045B14D, 0x0045B198);
// name, size, bases and virtual order are retail facts given below.
// AimWeaponBehavior.h: the AimWeaponBehavior module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0045B226 calls UpdateModule first; it stores the
// tables 0x00C418CC at +0x0, 0x00BEFF90 at +0xC, 0x00C418C0 at +0x10. Members are what xfer (0x0045B153) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x0045B14D ?name@AimWeaponBehavior@@QBEPBDXZ
//   +0x10 slot 0 -> 0x0045B297 ?update@AimWeaponBehavior@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?, slot10 ?.
#pragma once
#ifndef BFME2_AIM_WEAPON_BEHAVIOR_H
#define BFME2_AIM_WEAPON_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AimWeaponBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AimWeaponBehavior : public UpdateModule
{
public:
	char const * name() const;	// 0x0045B14D
	AimWeaponBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~AimWeaponBehavior();

protected:
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x20;                                // +0x20; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x24
typedef char AimWeaponBehavior_size[(sizeof(AimWeaponBehavior) == 0x24) ? 1 : -1];
typedef char AimWeaponBehavior_off_m_unk0x20[(offsetof(AimWeaponBehavior, m_unk0x20) == 0x20) ? 1 : -1];

#endif
