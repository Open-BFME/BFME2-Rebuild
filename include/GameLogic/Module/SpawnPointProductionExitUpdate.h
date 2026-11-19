// SpawnPointProductionExitUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 6 rows that MATCH against it on main (0x004A3B8F, 0x004A3BD6, 0x004A3CEA, 0x004A3E3E, 0x004A3F04, 0x004A3F3B);
// name, size, bases and virtual order are retail facts given below.
// SpawnPointProductionExitUpdate.h: the SpawnPointProductionExitUpdate module. SIZE 0xF4 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A3C1B calls UpdateModule first; it stores the
// tables 0x00C430B4 at +0x20, 0x00C5259C at +0x0, 0x00C4B1F0 at +0xC, 0x00C4B1E0 at +0x10, 0x00C5256C at +0x20. Members are what xfer (0x004A3B95) transfers after UpdateModule::xfer,
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
//   +0x0 slot 2 -> 0x004A3B8F ?name@SpawnPointProductionExitUpdate@@QBEPBDXZ
//   +0xC slot 20 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 49 -> 0x00487F5A ?name@Rva00487F5ANamed@@QBEPBDXZ
//   +0xC slot 51 -> 0x00487F82 ?rva000487F82@DefaultProductionExitUpdate@@SA?AW4NameKeyType@@XZ
//   +0xC slot 60 -> 0x004A6642 ?getExitPosition@SupplyCenterProductionExitUpdate@@UBE_NAAUCoord3D@@@Z
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
//   +0x20 slot 1 -> 0x004A3F3B ?reserveDoorForExit@SpawnPointProductionExitUpdate@@UAE?AW4ExitDoorType@@PBVThingTemplate@@PAVObject@@@Z
//   +0x20 slot 2 -> 0x004A3E3E ?exitObjectViaDoor@SpawnPointProductionExitUpdate@@UAEXPAVObject@@W4ExitDoorType@@@Z
// Not established: interface tables at +0x20 (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_SPAWN_POINT_PRODUCTION_EXIT_UPDATE_H
#define BFME2_SPAWN_POINT_PRODUCTION_EXIT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ExitInterface.h"

class Thing;
class ModuleData;
class Xfer;
class ThingTemplate;
class Object;

// UpdateModule ends at 0x20; ExitInterface's table is the subobject at +0x20 (retail
// 0x00C430B4; slot 1 reserveDoorForExit 0x004A3F3B, slot 2 exitObjectViaDoor 0x004A3E3E).
class SpawnPointProductionExitUpdate : public UpdateModule, public ExitInterface
{
public:
	SpawnPointProductionExitUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const; // 0x004A3B8F

	// ExitInterface
	virtual ExitDoorType reserveDoorForExit(const ThingTemplate *objType, Object *specificObject); // 0x004A3F3B
	virtual void exitObjectViaDoor(Object *newObj, ExitDoorType exitDoor);	// 0x004A3E3E

protected:
	virtual void xfer(Xfer *xfer);
	virtual ~SpawnPointProductionExitUpdate();

private:
	void initializeBonePositions();	// 0x004A3CEA
	void revalidateOccupiers();		// 0x004A3F04

public:
	Bool m_bonePositionsReady;							// +0x24; reserveDoorForExit tests it (0x004A3F3B)
	Int m_occupantCount;								// +0x28; revalidateOccupiers 0x004A3F04 loops to it
	Coord3D m_doorPositions[10];						// +0x2C; exitObjectViaDoor 0x004A3E3E copies entry i (12 bytes)
	Real m_doorAngles[10];								// +0xA4; exitObjectViaDoor reads entry i as the exit angle
	ObjectID m_occupants[10];							// +0xCC; revalidateOccupiers and reserveDoorForExit read them
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0xF4
typedef char SpawnPointProductionExitUpdate_size[(sizeof(SpawnPointProductionExitUpdate) == 0xF4) ? 1 : -1];

#endif
