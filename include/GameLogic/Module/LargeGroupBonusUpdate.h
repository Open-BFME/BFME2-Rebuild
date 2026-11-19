// LargeGroupBonusUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 3 rows that MATCH against it on main (0x0048FFD4, 0x00490031, 0x004901B5);
// name, size, bases and virtual order are retail facts given below.
// LargeGroupBonusUpdate.h: the LargeGroupBonusUpdate module. SIZE 0x2C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004900B1 calls UpdateModule first; it stores the
// tables 0x00BE2B78 at +0x20, 0x00C4D124 at +0x0, 0x00C4D068 at +0xC, 0x00C4D058 at +0x10, 0x00C4D04C at +0x20. Members are what xfer (0x00490031) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00490019 ?name@Rva00490019Named@@QBEPBDXZ
//   +0x0 slot 6 -> 0x004901B5 ?rva004901B5@LargeGroupBonusUpdate@@QAEXXZ
//   +0xC slot 29 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x00490225 ?update@LargeGroupBonusUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 0 -> 0x001DAF7D ?get@Rva001DAF7DByteField@@QBEEXZ
//   +0x20 slot 1 -> 0x0049002D ?get@Rva0049002DByteField@@QBEEXZ
//   +0x20 slot 2 -> 0x0049001F ?isTrueHordeMember@HordeUpdate@@UBE_NXZ
//   +0x20 slot 4 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 5 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
// Not established: interface tables at +0x20 (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_LARGE_GROUP_BONUS_UPDATE_H
#define BFME2_LARGE_GROUP_BONUS_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char LargeGroupBonusUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class LargeGroupBonusUpdate : public UpdateModule
{
public:
	LargeGroupBonusUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00490225)
	void notify(Int notice);					// 0x0049015C (the body needs the unnamed ICF fold at 0x005508E2; not written)
	void rva004901B5();							// 0x004901B5: notify(0)

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x20[0x4];
	UnsignedInt m_unk0x24;                         // +0x24; xfer UnsignedInt
	Bool m_unk0x28;                                // +0x28; xfer Bool
	Bool m_unk0x29;                                // +0x29; xfer Bool
	Bool m_unk0x2A;                                // +0x2A; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x2C
typedef char LargeGroupBonusUpdate_size[(sizeof(LargeGroupBonusUpdate) == 0x2C) ? 1 : -1];
typedef char LargeGroupBonusUpdate_off_m_unk0x24[(offsetof(LargeGroupBonusUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char LargeGroupBonusUpdate_off_m_unk0x28[(offsetof(LargeGroupBonusUpdate, m_unk0x28) == 0x28) ? 1 : -1];
typedef char LargeGroupBonusUpdate_off_m_unk0x29[(offsetof(LargeGroupBonusUpdate, m_unk0x29) == 0x29) ? 1 : -1];
typedef char LargeGroupBonusUpdate_off_m_unk0x2A[(offsetof(LargeGroupBonusUpdate, m_unk0x2A) == 0x2A) ? 1 : -1];

#endif
