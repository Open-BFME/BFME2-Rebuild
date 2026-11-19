// GiantBirdSlowDeathBehavior.h: the GiantBirdSlowDeathBehavior module. SIZE 0x68 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00461E58 calls SlowDeathBehavior first; it stores the
// tables 0x00C42E64 at +0x0, 0x00C42DA8 at +0xC, 0x00C42D98 at +0x10, 0x00C42030 at +0x20, 0x00C42D88 at +0x24. Members are what xfer (0x00461F34) transfers after SlowDeathBehavior::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x00461EE5 ?name@Rva00461EE5Named@@QBEPBDXZ
//   +0x0 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 16 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 17 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 27 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 29 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 43 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 0 -> 0x00461F12 ?update@GiantBirdSlowDeathBehavior@@UAE?AW4UpdateSleepTime@@XZ
//   +0x24 slot 0 -> 0x0046200E ?beginSlowDeath@GiantBirdSlowDeathBehavior@@UAEXPBVDamageInfo@@@Z
//   +0x24 slot 5 -> 0x004DF396 ?getDisabledTypesToProcess@FireWeaponWhenDeadBehavior@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x24 slot 6 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
#pragma once
#ifndef BFME2_GIANT_BIRD_SLOW_DEATH_BEHAVIOR_H
#define BFME2_GIANT_BIRD_SLOW_DEATH_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SlowDeathBehavior.h"
#include "Common/Coord3D.h"

class Thing;
class ModuleData;
class Xfer;

// SlowDeathBehavior ends at 0x50; this class's members follow it.
typedef char GiantBirdSlowDeathBehavior_base_size[(sizeof(SlowDeathBehavior) == 0x50) ? 1 : -1];

class GiantBirdSlowDeathBehavior : public SlowDeathBehavior
{
public:
	GiantBirdSlowDeathBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00461F12)
	virtual void beginSlowDeath(const DamageInfo *damageInfo);	// override: beginSlowDeath slot 0 at +0x24 (0x0046200E)

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x50;                         // +0x50; xfer UnsignedInt
	Coord3D m_unk0x54;                             // +0x54; xfer Coord3D
	Real m_unk0x60;                                // +0x60; xfer Real
	Bool m_unk0x64;                                // +0x64; xfer Bool
protected:
	virtual void loadPostProcess() { SlowDeathBehavior::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SlowDeathBehavior's (h05)
};
// SIZE 0x68
typedef char GiantBirdSlowDeathBehavior_size[(sizeof(GiantBirdSlowDeathBehavior) == 0x68) ? 1 : -1];
typedef char GiantBirdSlowDeathBehavior_off_m_unk0x50[(offsetof(GiantBirdSlowDeathBehavior, m_unk0x50) == 0x50) ? 1 : -1];
typedef char GiantBirdSlowDeathBehavior_off_m_unk0x54[(offsetof(GiantBirdSlowDeathBehavior, m_unk0x54) == 0x54) ? 1 : -1];
typedef char GiantBirdSlowDeathBehavior_off_m_unk0x60[(offsetof(GiantBirdSlowDeathBehavior, m_unk0x60) == 0x60) ? 1 : -1];
typedef char GiantBirdSlowDeathBehavior_off_m_unk0x64[(offsetof(GiantBirdSlowDeathBehavior, m_unk0x64) == 0x64) ? 1 : -1];

#endif
