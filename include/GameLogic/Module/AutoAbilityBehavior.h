// AutoAbilityBehavior.h: the AutoAbilityBehavior module. SIZE 0x34 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0045A78F calls UpdateModule first; it stores the
// tables 0x00C4175C at +0x0, 0x00BEFF90 at +0xC, 0x00C41750 at +0x10. Members are what xfer (0x0045A291) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x0045A963 ?update@AutoAbilityBehavior@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 1 -> 0x004A2D67 ?getDisabledTypesToProcess@AIUpdateInterface@@UBE?AV?$BitFlags@$0L@@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_AUTO_ABILITY_BEHAVIOR_H
#define BFME2_AUTO_ABILITY_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/BitFlags11.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AutoAbilityBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AutoAbilityBehavior : public UpdateModule
{
public:
	AutoAbilityBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~AutoAbilityBehavior();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0045A963)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	AsciiString m_unk0x20;                         // +0x20; xfer AsciiString
	Coord3D m_unk0x24;                             // +0x24; xfer Coord3D
	Bool m_unk0x30;                                // +0x30; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
public:
	virtual DisabledMaskType getDisabledTypesToProcess() const { return DisabledMaskType(DisabledMaskType::kInit, 3); }	// UpdateModuleInterface slot 1: retail 0x004A2D67 (DISABLED_HELD only; shared body, filed under AIUpdateInterface)
};
// SIZE 0x34
typedef char AutoAbilityBehavior_size[(sizeof(AutoAbilityBehavior) == 0x34) ? 1 : -1];
typedef char AutoAbilityBehavior_off_m_unk0x20[(offsetof(AutoAbilityBehavior, m_unk0x20) == 0x20) ? 1 : -1];
typedef char AutoAbilityBehavior_off_m_unk0x24[(offsetof(AutoAbilityBehavior, m_unk0x24) == 0x24) ? 1 : -1];
typedef char AutoAbilityBehavior_off_m_unk0x30[(offsetof(AutoAbilityBehavior, m_unk0x30) == 0x30) ? 1 : -1];

#endif
