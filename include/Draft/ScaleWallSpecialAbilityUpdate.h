// DRAFT (h05 modgen): generated from constructor RVA 0x00494DCA and xfer RVA 0x00494E57; no function
// matches against it yet, so it stays in Draft until one does.
// ScaleWallSpecialAbilityUpdate.h: the ScaleWallSpecialAbilityUpdate module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00494DCA calls SpecialAbilityUpdate first; it stores the
// tables 0x00C4EB40 at +0x0, 0x00C4D640 at +0xC, 0x00C4EB30 at +0x10, 0x00C4EB0C at +0x20. Members are what xfer (0x00494E57) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x00494DFB ?Rva00494DFBGet@@YAHXZ
//   +0x0 slot 13 -> 0x00494E4D ?rva00494E4D@Rva00494E4D@@QAEX_N0@Z
//   +0x0 slot 16 -> 0x00494E21 ?rva00494E21@Rva00494E21@@QAE_NXZ
//   +0x0 slot 17 -> 0x00494E26 ?triggerAbilityEffect@ScaleWallSpecialAbilityUpdate@@UAEXXZ
//   +0x0 slot 21 -> 0x004925DB ?startPacking@WeaponFireSpecialAbilityUpdate@@UAEX_N@Z
//   +0x0 slot 22 -> 0x00494E48 ?rva00494E48@Rva00494E48@@QAEXXZ
//   +0x10 slot 0 -> 0x00494E52 ?rva00494E52@Rva00494E52@@QAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 8 -> 0x005CB9FA ?_bfme_ro_v50@RenderObjClass@@UAE_NH@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@SpecialAbilityUpdate@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_SCALE_WALL_SPECIAL_ABILITY_UPDATE_H
#define BFME2_SCALE_WALL_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char ScaleWallSpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class ScaleWallSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	ScaleWallSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// 0x00494E26

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x88
typedef char ScaleWallSpecialAbilityUpdate_size[(sizeof(ScaleWallSpecialAbilityUpdate) == 0x88) ? 1 : -1];

#endif
