// DelayedWeaponSetUpgradeUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x0048828E);
// name, size, bases and virtual order are retail facts given below.
// DelayedWeaponSetUpgradeUpdate.h: the DelayedWeaponSetUpgradeUpdate module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004882E4 calls UpdateModule first; it stores the
// tables 0x00C6FFFC at +0x20, 0x00C4B2F8 at +0x0, 0x00BEFF90 at +0xC, 0x00C4B2EC at +0x10, 0x00C4B2E4 at +0x20. Members are what xfer (0x004DF81B) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00488288 ?name@Rva00488288Named@@QBEPBDXZ
//   +0x0 slot 13 -> 0x0048846F ?update@DeletionUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x0 slot 14 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x0 slot 15 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x0 slot 21 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 25 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 26 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 28 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x10 slot 0 -> 0x000910FB ?Create@GameFileClass@@UAEHXZ
//   +0x20 slot 0 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x20 slot 1 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
// Not established: interface tables at +0x20 (their vptrs sit in the gaps there).
#pragma once
#ifndef BFME2_DELAYED_WEAPON_SET_UPGRADE_UPDATE_H
#define BFME2_DELAYED_WEAPON_SET_UPGRADE_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char DelayedWeaponSetUpgradeUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class DelayedWeaponSetUpgradeUpdate : public UpdateModule
{
public:
	DelayedWeaponSetUpgradeUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);
	virtual ~DelayedWeaponSetUpgradeUpdate();

public:
	UnsignedByte m_gap0x20[0x4];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x24
typedef char DelayedWeaponSetUpgradeUpdate_size[(sizeof(DelayedWeaponSetUpgradeUpdate) == 0x24) ? 1 : -1];

#endif
