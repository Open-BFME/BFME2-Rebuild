// WallHubBehavior.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x00452DA3, 0x00452F25);
// name, size, bases and virtual order are retail facts given below.
// WallHubBehavior.h: the WallHubBehavior module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00452E27 calls UpdateModule first; it stores the
// tables 0x00C633C0 at +0x20, 0x00C3FDBC at +0x0, 0x00BEFF90 at +0xC, 0x00C4B1E0 at +0x10, 0x00C3FD80 at +0x20. Members are what xfer (0x00452F25) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x20 slot 15 -> 0x004FA5F4 ?getAIUpdateInterface@AIUpdateInterface@@UAEPAV1@XZ
//   +0x20 slot 17 -> 0x004FA66C ?rva004FA66C@Rva004FA66C@@QAEHXZ
//   +0x20 slot 18 -> 0x004FAE34 ?rva004FAE34@Rva004FAE34Owner@@QAEXPAV?$vector@W4ScienceType@@V?$allocator@W4ScienceType@@@_STL@@@_STL@@@Z
//   +0x20 slot 19 -> 0x004FB128 ?rva004FB128@Rva004FB128@@QAE_NHPAURva004FAFF2@@@Z
//   +0x20 slot 20 -> 0x004FAEAA ?rva004FAEAA@Rva004FA992Owner@@QAE_NW4ScienceType@@@Z
//   +0x20 slot 21 -> 0x004FA5E4 ?rva004FA5E4@Rva004FA992Owner@@QAE_NPBURva004FA5D4Source@@@Z
//   +0x20 slot 22 -> 0x004FA83E ?canQueueUnit@LivingWorldBuildingNuggetSpawnArmy@@UAE_NW4ObjectID@@@Z
//   +0x20 slot 23 -> 0x004FA5D4 ?rva004FA5D4@Rva004FA992Owner@@QAE_NPBURva004FA5D4Source@@@Z
//   +0x20 slot 24 -> 0x004FA9B1 ?rva004FA9B1@Rva004FA992Owner@@QAEXH@Z
//   +0x20 slot 25 -> 0x004FA686 ?rva004FA686@Rva004FA992@@QAEXW4ObjectID@@@Z
//   +0x20 slot 26 -> 0x0059E826 ?rva0059E826@Rva0059E848@@QAE_NH@Z
//   +0x20 slot 27 -> 0x0030D377 ?getDataLength@NetWrapperCommandMsg@@QAEIXZ
//   +0x20 slot 28 -> 0x005C4AD9 ?get@Rva005C4AD9LeaField@@QBEPAXXZ
//   +0x20 slot 29 -> 0x004FA6B5 ?rva004FA6B5@Rva004FA992@@QAEHXZ
//   +0x20 slot 31 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 32 -> 0x004FA61D ?name@Rva004FA61DNamed@@QBEPBDXZ
//   +0x20 slot 33 -> 0x004FAF73 ?rva004FAF73@Rva004FAF73@@QAEXPAVXfer@@@Z
//   +0x20 slot 34 -> 0x004FAA81 ?runProductionPhase@LivingWorldBuildingNuggetSpawnArmy@@UAEXXZ
//   +0x20 slot 35 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x20 slot 37 -> 0x000CA841 ?vslot13@UpgradeSoundSelectorClientBehavior@@UAEPAXXZ
//   +0x20 slot 39 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00452D72 ?name@Rva00452D72Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004A6563 ?update@AssistedTargetingUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 0 -> 0x00452D78 ?Rva00452D78Adjust@@YIPAXPAX@Z
//   +0x20 slot 1 -> 0x00452F7C ?rva00452F7C@Rva00452F7C@@QAEPBVThingTemplate@@I@Z
//   +0x20 slot 2 -> 0x00452EDD ?rva00452EDD@Rva00452EDD@@UAEPAXXZ
//   +0x20 slot 3 -> 0x00452EF5 ?rva00452EF5@Rva00452EDD@@UAEPAXXZ
//   +0x20 slot 4 -> 0x00452F0D ?rva00452F0D@Rva00452EDD@@UAEPAXXZ
//   +0x20 slot 8 -> 0x00452D86 ?Rva00452D86Get@@YAHXZ
//   +0x20 slot 9 -> 0x00452D8C ?rva00452D8C@Rva00452D8C@@QAEXH@Z
//   +0x20 slot 10 -> 0x00452D98 ?rva00452D98@Rva00452D8C@@QAEXXZ
//   +0x20 slot 11 -> 0x00452D82 ?getMinimumValue@GameClientRandomVariable@@QBEMXZ
//   +0x20 slot 12 -> 0x00452E00 ?getValue@Rva00452E00@@UAEHPAM@Z
//   +0x20 slot 14 -> 0x00452DE8 ?get@Rva00452DE8PtrChaseField@@QBEHXZ
//   +0x20 slot 24 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 27 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
// Not established: interface tables at +0x20 (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_WALL_HUB_BEHAVIOR_H
#define BFME2_WALL_HUB_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char WallHubBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class WallHubBehavior : public UpdateModule
{
public:
	WallHubBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x20[0x8];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
typedef char WallHubBehavior_size[(sizeof(WallHubBehavior) == 0x28) ? 1 : -1];

#endif
