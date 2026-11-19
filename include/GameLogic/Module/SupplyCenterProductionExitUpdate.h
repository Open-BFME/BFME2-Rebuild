// SupplyCenterProductionExitUpdate.h: the SupplyCenterProductionExitUpdate module. SIZE 0x34 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A65AE calls UpdateModule first; it stores the
// tables 0x00C430B4 at +0x20, 0x00C52D30 at +0x0, 0x00C4B1F0 at +0xC, 0x00C4B1E0 at +0x10, 0x00C52D00 at +0x20. Members are what xfer (0x0048805B) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x20 slot 5 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x20 slot 6 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x20 slot 9 -> 0x004624A7 ?getNaturalRallyPoint@ExitInterface@@UBE_NAAUCoord3D@@_N@Z
//   +0x20 slot 10 -> 0x004624C1 ?rva004624C1@Rva004624C1@@QAE_NPAURva00597372Triple@@PAM@Z
//   +0x20 slot 11 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004A655D ?name@Rva004A655DNamed@@QBEPBDXZ
//   +0x0 slot 13 -> 0x004A6642 ?getExitPosition@SupplyCenterProductionExitUpdate@@UBE_NAAUCoord3D@@@Z
//   +0xC slot 20 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 49 -> 0x00487F5A ?name@Rva00487F5ANamed@@QBEPBDXZ
//   +0xC slot 51 -> 0x00487F82 ?rva000487F82@DefaultProductionExitUpdate@@SA?AW4NameKeyType@@XZ
//   +0xC slot 61 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0xC slot 63 -> 0x000910FB ?Create@GameFileClass@@UAEHXZ
//   +0xC slot 64 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0xC slot 65 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0xC slot 68 -> 0x00488288 ?name@Rva00488288Named@@QBEPBDXZ
//   +0xC slot 75 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0xC slot 76 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0xC slot 78 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0xC slot 79 -> 0x0048846F ?update@DeletionUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 0 -> 0x004A6563 ?update@AssistedTargetingUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 1 -> 0x005748B2 ?Write@GameFileClass@@UAEHPBXH@Z
//   +0x20 slot 2 -> 0x004A6702 ?exitObjectViaDoor@SupplyCenterProductionExitUpdate@@UAEXPAVObject@@W4ExitDoorType@@@Z
//   +0x20 slot 7 -> 0x00487F60 ?setRallyPoint@DefaultProductionExitUpdate@@UAEXPBUCoord3D@@@Z
//   +0x20 slot 8 -> 0x00487F75 ?getRallyPoint@DefaultProductionExitUpdate@@UBEPBUCoord3D@@XZ
//   +0x20 slot 9 -> 0x004A027A ?getNaturalRallyPoint@QueueProductionExitUpdate@@UBE_NAAUCoord3D@@_N@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
// Interfaces after the base (own table -> class): +0x20 ExitInterface.
#pragma once
#ifndef BFME2_SUPPLY_CENTER_PRODUCTION_EXIT_UPDATE_H
#define BFME2_SUPPLY_CENTER_PRODUCTION_EXIT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ExitInterface.h"
#include "Common/Coord3D.h"

class Coord3D;
class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char SupplyCenterProductionExitUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class SupplyCenterProductionExitUpdate : public UpdateModule, public ExitInterface
{
public:
	SupplyCenterProductionExitUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual Bool getExitPosition(Coord3D &exitPosition, Real &exitAngle) const;	// override: getExitPosition slot 60 at +0xC (0x004A6642)
	virtual void exitObjectViaDoor(Object *newObj, ExitDoorType exitDoor);	// override: exitObjectViaDoor slot 2 at +0x20 (0x004A6702)
	virtual bool getExitPosition(Coord3D &) const;	// slot 13, 0x004A6642 (own new slot)

protected:
	virtual void xfer(Xfer *xfer);

public:
	Coord3D m_unk0x24;                             // +0x24; xfer Coord3D
	Bool m_unk0x30;                                // +0x30; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x34
typedef char SupplyCenterProductionExitUpdate_size[(sizeof(SupplyCenterProductionExitUpdate) == 0x34) ? 1 : -1];
typedef char SupplyCenterProductionExitUpdate_off_m_unk0x24[(offsetof(SupplyCenterProductionExitUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char SupplyCenterProductionExitUpdate_off_m_unk0x30[(offsetof(SupplyCenterProductionExitUpdate, m_unk0x30) == 0x30) ? 1 : -1];

#endif
