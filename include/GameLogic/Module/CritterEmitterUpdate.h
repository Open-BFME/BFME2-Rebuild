// CritterEmitterUpdate.h: the CritterEmitterUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C8D86 calls UpdateModule first; it stores the
// tables 0x00C40818 at +0x20, 0x00C5E8E8 at +0x0, 0x00C5A260 at +0xC, 0x00C5E8DC at +0x10, 0x00C5E8C4 at +0x20. Members are what xfer (0x004C8E42) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0xC slot 1 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 49 -> 0x004BBEFD ?name@AODCrushCollide@@QBEPBDXZ
//   +0xC slot 61 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0xC slot 63 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0xC slot 64 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0xC slot 74 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0xC slot 76 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0xC slot 90 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 0 -> 0x004C8DE2 ?update@CritterEmitterUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 0 -> 0x004C8E83 ?onCollide@CritterEmitterUpdate@@UAEXPAVObject@@PBVCoord3D@@1@Z
//   +0x20 slot 1 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x20 slot 7 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 8 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x20 slot 18 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 21 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
// Interfaces after the base (own table -> class): +0x20 CollideModuleInterface.
#pragma once
#ifndef BFME2_CRITTER_EMITTER_UPDATE_H
#define BFME2_CRITTER_EMITTER_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/CollideModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char CritterEmitterUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class CritterEmitterUpdate : public UpdateModule, public CollideModuleInterface
{
public:
	virtual CollideModuleInterface *getCollide() { return this; }
	CritterEmitterUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004C8DE2)
	virtual void onCollide(Object *other, const Coord3D *loc, const Coord3D *normal);	// override: onCollide slot 0 at +0x20 (0x004C8E83)
	virtual Bool wouldLikeToCollideWith(const Object *) const { return false; }
	virtual Bool isHijackedVehicleCrateCollide() const { return false; }
	virtual Bool isCarBombCrateCollide() const { return false; }
	virtual Bool isRailroad() const { return false; }
	virtual Bool isSalvageCrateCollide() const { return false; }

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x24;                                // +0x24; xfer Bool
	Bool m_unk0x25;                                // +0x25; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
typedef char CritterEmitterUpdate_size[(sizeof(CritterEmitterUpdate) == 0x28) ? 1 : -1];
typedef char CritterEmitterUpdate_off_m_unk0x24[(offsetof(CritterEmitterUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char CritterEmitterUpdate_off_m_unk0x25[(offsetof(CritterEmitterUpdate, m_unk0x25) == 0x25) ? 1 : -1];

#endif
