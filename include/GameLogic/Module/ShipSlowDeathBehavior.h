// ShipSlowDeathBehavior.h: the ShipSlowDeathBehavior module. SIZE 0x5C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0045E9AA calls SlowDeathBehavior first; it stores the
// tables 0x00C423FC at +0x0, 0x00C42DA8 at +0xC, 0x00C423F0 at +0x10, 0x00C42030 at +0x20, 0x00C423E0 at +0x24. Members are what xfer (0x0045ED0E) transfers after SlowDeathBehavior::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x0045E9F4 ?Rva45E9F4Get@@YAHXZ
//   +0x10 slot 0 -> 0x0045EB02 ?update@ShipSlowDeathBehavior@@UAE?AW4UpdateSleepTime@@XZ
//   +0x24 slot 0 -> 0x0045EA66 ?beginSlowDeath@ShipSlowDeathBehavior@@UAEXPBVDamageInfo@@@Z
//   +0x24 slot 5 -> 0x004DF396 ?getDisabledTypesToProcess@FireWeaponWhenDeadBehavior@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x24 slot 6 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x24 slot 12 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x24 slot 16 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x24 slot 17 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x24 slot 19 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_SHIP_SLOW_DEATH_BEHAVIOR_H
#define BFME2_SHIP_SLOW_DEATH_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SlowDeathBehavior.h"

class Thing;
class ModuleData;
class Xfer;

// SlowDeathBehavior ends at 0x50; this class's members follow it.
typedef char ShipSlowDeathBehavior_base_size[(sizeof(SlowDeathBehavior) == 0x50) ? 1 : -1];

class ShipSlowDeathBehavior : public SlowDeathBehavior
{
public:
	ShipSlowDeathBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~ShipSlowDeathBehavior();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0045EB02)
	virtual void beginSlowDeath(const DamageInfo *damageInfo);	// override: beginSlowDeath slot 0 at +0x24 (0x0045EA66)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Real m_pitchRate;		// +0x50; beginSlowDeath 0x0045EA66 writes it, update 0x0045EB02 reads it
	Real m_rollRate;		// +0x54; beginSlowDeath 0x0045EA66 writes it, update 0x0045EB02 reads it
	Real m_rotation;		// +0x58; beginSlowDeath 0x0045EA66 stores 0.0 here
protected:
	virtual void loadPostProcess() { SlowDeathBehavior::loadPostProcess(); }	// retail slot 0x00483CEF, a jmp to 0x00497805 (h05, gen3 vtable-content list)
};
// SIZE 0x5C
typedef char ShipSlowDeathBehavior_size[(sizeof(ShipSlowDeathBehavior) == 0x5C) ? 1 : -1];

#endif
