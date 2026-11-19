// SiegeDockingBehavior.h: promoted from include/Draft by h05 (round 10). Proven by 3 rows that MATCH against it on main (0x004599DF, 0x004599E5, 0x0045A16D);
// name, size, bases and virtual order are retail facts given below.
// SiegeDockingBehavior.h: the SiegeDockingBehavior module. SIZE 0x34 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0045996C calls UpdateModule first; it stores the
// tables 0x00C4EF80 at +0x20, 0x00C414DC at +0x0, 0x00C41420 at +0xC, 0x00C41414 at +0x10, 0x00C41404 at +0x20. Members are what xfer (0x0045A02F) transfers after UpdateModule::xfer,
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
//   +0x0 slot 1 -> 0x0045A16D ?loadPostProcess@SiegeDockingBehavior@@MAEXXZ
//   +0xC slot 34 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x0045A1B6 ?rva0045A1B6@Rva0045A1B6@@QAEIXZ
//   +0x10 slot 3 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 12 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x10 slot 46 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 47 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 59 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 62 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 0 -> 0x004543E3 ?get@Rva004543A9LeaNegSbbField@@WBE@BEHXZ
//   +0x20 slot 1 -> 0x00459A2A ?rva00459A2A@SiegeDockingBehavior@@UAE_NW4ObjectID@@PAUCoord3D@@@Z
//   +0x20 slot 2 -> 0x00459CC0 ?rva00459CC0@SiegeDockingBehavior@@UAEHW4ObjectID@@@Z
//   +0x20 slot 3 -> 0x00459C58 ?rva00459C58@SiegeDockingBehavior@@UAE_NW4ObjectID@@@Z
// Not established: interface tables at +0x20 (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: slot8 ?, UnsignedInt ?, Int ?, Coord3D ?, Coord3D ?, UnsignedInt ?, Int ?, Coord3D ?, Coord3D ?.
#pragma once
#ifndef BFME2_SIEGE_DOCKING_BEHAVIOR_H
#define BFME2_SIEGE_DOCKING_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char SiegeDockingBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class SiegeDockingBehavior : public UpdateModule
{
public:
	SiegeDockingBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~SiegeDockingBehavior();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();	// 0x0045A16D: rva00459E05, then the base's

private:
	void rva00459E05();	// 0x00459E05 (loadPostProcess calls it first)

public:
	UnsignedByte m_gap0x20[0x10];
	Bool m_unk0x30;                                // +0x30; xfer Bool
};
// SIZE 0x34
typedef char SiegeDockingBehavior_size[(sizeof(SiegeDockingBehavior) == 0x34) ? 1 : -1];
typedef char SiegeDockingBehavior_off_m_unk0x30[(offsetof(SiegeDockingBehavior, m_unk0x30) == 0x30) ? 1 : -1];

#endif
