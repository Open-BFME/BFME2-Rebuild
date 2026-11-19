// InvisibilityUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x004A38BF, 0x004A3981);
// name, size, bases and virtual order are retail facts given below.
// InvisibilityUpdate.h: the InvisibilityUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A382D calls UpdateModule first; it stores the
// tables 0x00C4EF80 at +0x20, 0x00C52534 at +0x0, 0x00BEFF90 at +0xC, 0x00C52528 at +0x10, 0x00C52518 at +0x20. Members are what xfer (0x004A3981) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x20 slot 4 -> 0x00495DC6 ?get@Rva00495DC6PtrChaseField@@QBEHXZ
//   +0x20 slot 5 -> 0x00495DCD ?get@Rva00495DCDByteChaseField@@QBEEXZ
//   +0x20 slot 6 -> 0x00495C6E ?rva00495C6E@AutoPickUpUpdate@@UAEXXZ
//   +0x20 slot 7 -> 0x0058B826 ?getIsQuitting@FrameDataManager@@QAE_NXZ
//   +0x20 slot 8 -> 0x00495E01 ?update@AutoPickUpUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 9 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 10 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004A3871 ?Rva004A3871Get@@YAHXZ
//   +0x0 slot 5 -> 0x004A390B ?Rva004A390B@InvisibilityUpdate@@MAEXXZ
//   +0x0 slot 10 -> 0x004A3877 ?rva004A3877@InvisibilityUpdate@@UAEPAVRva004A3A09Iface@@XZ
//   +0x10 slot 0 -> 0x004A3A20 ?update@InvisibilityUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 1 -> 0x004A2D67 ?getDisabledTypesToProcess@AIUpdateInterface@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 0 -> 0x004A3A09 ?rva004A3A09@InvisibilityUpdate@@UAE_NXZ
//   +0x20 slot 1 -> 0x004A39BE ?get@Rva004A39BEAddImm32Field@@QBEHXZ
//   +0x20 slot 2 -> 0x004A39C7 ?get@Rva004A39C7AddImm32Field@@QBEHXZ
//   +0x20 slot 3 -> 0x004A3904 ?get@Rva004A3904AddDwordField@@QBEHXZ
//   +0x20 slot 13 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 16 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 19 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
// Not established: interface tables at +0x20 (their vptrs sit in the gaps there).
#pragma once
#ifndef BFME2_INVISIBILITY_UPDATE_H
#define BFME2_INVISIBILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char InvisibilityUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class InvisibilityUpdate : public UpdateModule
{
public:
	InvisibilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004A3A20)
	void rva004A39D0(Bool on);	// 0x004A39D0 (map: placeholder; ToggleHiddenSpecialAbilityUpdate::turnOn 0x004AE394 passes true, turnOff 0x004AE2D0 false)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual ~InvisibilityUpdate();

public:
	UnsignedByte m_gap0x20[0x4];
	Bool m_unk0x24;                                // +0x24; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
typedef char InvisibilityUpdate_size[(sizeof(InvisibilityUpdate) == 0x28) ? 1 : -1];
typedef char InvisibilityUpdate_off_m_unk0x24[(offsetof(InvisibilityUpdate, m_unk0x24) == 0x24) ? 1 : -1];

#endif
