// MonsterDockUpdate.h: the MonsterDockUpdate module. SIZE 0x90 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A139A calls DockUpdate first; it stores the
// tables 0x00C51D74 at +0x0, 0x00C51B20 at +0xC, 0x00C51D68 at +0x10, 0x00C51D18 at +0x20. Members are what xfer (0x004A14C2) transfers after DockUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004A0E47 ?loadPostProcess@MonsterDockUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x004A13E0 ?Rva004A13E0Get@@YAHXZ
//   +0x0 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 16 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 17 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 27 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 29 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 43 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0xC slot 22 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 49 -> 0x004A0E06 ?Rva004A0E06Get@@YAHXZ
//   +0x20 slot 8 -> 0x004A159E ?onApproachReached@MonsterDockUpdate@@UAEXPAVObject@@@Z
//   +0x20 slot 9 -> 0x004A14A6 ?onEnterReached@MonsterDockUpdate@@UAEXPAVObject@@@Z
//   +0x20 slot 10 -> 0x004A1610 ?rva004A1610@MonsterDockUpdate@@UAEXPAVObject@@@Z
//   +0x20 slot 11 -> 0x004A17C1 ?onExitReached@MonsterDockUpdate@@UAEXPAVObject@@@Z
//   +0x20 slot 12 -> 0x004A1659 ?action@MonsterDockUpdate@@UAE_NPAVObject@@0@Z
//   +0x20 slot 14 -> 0x004A13E6 ?get@Rva004A13E6ByteField@@QBEEXZ
//   +0x20 slot 15 -> 0x004A13EA ?setDockOpen@MonsterDockUpdate@@UAEX_N@Z
//   +0x20 slot 16 -> 0x004A1475 ?check@Rva002CDBF0@@QAE_NPAVObject@@@Z
//   +0x20 slot 20 -> 0x00589F09 ?update@DockUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 21 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 22 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x20 slot 32 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 35 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_MONSTER_DOCK_UPDATE_H
#define BFME2_MONSTER_DOCK_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DockUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// DockUpdate ends at 0x88; this class's members follow it.
typedef char MonsterDockUpdate_base_size[(sizeof(DockUpdate) == 0x88) ? 1 : -1];

class Object;
class MonsterDockUpdate : public DockUpdate
{
public:
	virtual DockUpdateInterface *getDockUpdateInterface() { return this; }
	MonsterDockUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual Bool dockSlot16(Object *docker);
	virtual void onApproachReached(Object *docker);	// override: onApproachReached slot 8 at +0x20 (0x004A159E)
	virtual void onEnterReached(Object *docker);	// override: onEnterReached slot 9 at +0x20 (0x004A14A6)
	virtual void onExitReached(Object *docker);	// override: onExitReached slot 11 at +0x20 (0x004A17C1)
	virtual Bool action(Object *docker, Object *drone);	// override: action slot 12 at +0x20 (0x004A1659)
	virtual Bool isDockOpen();	// override: isDockOpen slot 14 at +0x20 (0x004A13E6, reads what setDockOpen stores)
	virtual void setDockOpen(Bool open);	// override: setDockOpen slot 15 at +0x20 (0x004A13EA)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual ~MonsterDockUpdate();
	virtual void loadPostProcess();	// override: loadPostProcess slot 1 at +0x0 (0x004A0E47)

public:
	Bool m_unk0x88;                                // +0x88; xfer Bool
	Bool m_unk0x89;                                // +0x89; xfer Bool
	Int m_unk0x8C;                                 // +0x8C; xfer Int
public:
	virtual void onDockReached(Object *docker);	// 0x004A1610, DockUpdateInterface slot 10; retail's table has this class's own (h05, gen3 vtable-content list)
};
// SIZE 0x90
typedef char MonsterDockUpdate_size[(sizeof(MonsterDockUpdate) == 0x90) ? 1 : -1];
typedef char MonsterDockUpdate_off_m_unk0x88[(offsetof(MonsterDockUpdate, m_unk0x88) == 0x88) ? 1 : -1];
typedef char MonsterDockUpdate_off_m_unk0x89[(offsetof(MonsterDockUpdate, m_unk0x89) == 0x89) ? 1 : -1];
typedef char MonsterDockUpdate_off_m_unk0x8C[(offsetof(MonsterDockUpdate, m_unk0x8C) == 0x8C) ? 1 : -1];

#endif
