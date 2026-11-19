// AODCrushCollide.h: the AODCrushCollide module. SIZE 0x30 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004BC00D calls UpdateModule first; it stores the
// tables 0x00C40818 at +0x20, 0x00C5A31C at +0x0, 0x00C5A260 at +0xC, 0x00C5A254 at +0x10, 0x00C5A23C at +0x20. Members are what xfer (0x004BBF03) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004BBEFD ?name@AODCrushCollide@@QBEPBDXZ
//   +0x0 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 16 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 17 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 27 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 29 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 43 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0xC slot 1 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x004BC2E6 ?update@AODCrushCollide@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 12 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x10 slot 59 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 62 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 0 -> 0x004BC0B8 ?onCollide@AODCrushCollide@@UAEXPAVObject@@PBUCoord3D@@1@Z
//   +0x20 slot 1 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x20 slot 7 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 8 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
// Interfaces after the base (own table -> class): +0x20 CollideModuleInterface.
#pragma once
#ifndef BFME2_A_O_D_CRUSH_COLLIDE_H
#define BFME2_A_O_D_CRUSH_COLLIDE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/CollideModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AODCrushCollide_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AODCrushCollide : public UpdateModule, public CollideModuleInterface
{
public:
	virtual CollideModuleInterface *getCollide() { return this; }
	AODCrushCollide(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;						// 0x004BBEFD (non-virtual, the literal)
	virtual ~AODCrushCollide();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004BC2E6)
	virtual void onCollide(Object *other, const Coord3D *loc, const Coord3D *normal);	// override: onCollide slot 0 at +0x20 (0x004BC0B8)

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x24;                         // +0x24; xfer UnsignedInt
	ObjectID m_unk0x28;                            // +0x28; xfer ObjectID
	Bool m_unk0x2C;                                // +0x2C; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
public:
	virtual Bool wouldLikeToCollideWith(const Object *other) const { return false; }	// CollideModuleInterface slot 1: retail 0x005CB9FF, `xor al,al; ret 4` (ICF-folded)
	virtual Bool isHijackedVehicleCrateCollide() const { return false; }	// slot 2: 0x0047A699 (ICF-folded `xor al,al; ret`)
	virtual Bool isCarBombCrateCollide() const { return false; }			// slot 3: 0x0047A699
	virtual Bool isRailroad() const { return false; }						// slot 4: 0x0047A699
	virtual Bool isSalvageCrateCollide() const { return false; }			// slot 5: 0x0047A699
};
// SIZE 0x30
typedef char AODCrushCollide_size[(sizeof(AODCrushCollide) == 0x30) ? 1 : -1];
typedef char AODCrushCollide_off_m_unk0x24[(offsetof(AODCrushCollide, m_unk0x24) == 0x24) ? 1 : -1];
typedef char AODCrushCollide_off_m_unk0x28[(offsetof(AODCrushCollide, m_unk0x28) == 0x28) ? 1 : -1];
typedef char AODCrushCollide_off_m_unk0x2C[(offsetof(AODCrushCollide, m_unk0x2C) == 0x2C) ? 1 : -1];

#endif
