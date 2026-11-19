// GiveOrRestoreUpgradeSpecialPower.h: the GiveOrRestoreUpgradeSpecialPower module. SIZE 0x90 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004CD0DC calls SpecialAbilityUpdate first; it stores the
// tables 0x00C5FA40 at +0x0, 0x00C5F980 at +0xC, 0x00C4DEE8 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004CD11B ?Rva004CD11BGet@@YAHXZ
//   +0x0 slot 17 -> 0x004CD1B9 ?triggerAbilityEffect@GiveOrRestoreUpgradeSpecialPower@@UAEXXZ
//   +0xC slot 39 -> 0x004FA5F4 ?getAIUpdateInterface@AIUpdateInterface@@UAEPAV1@XZ
#pragma once
#ifndef BFME2_GIVE_OR_RESTORE_UPGRADE_SPECIAL_POWER_H
#define BFME2_GIVE_OR_RESTORE_UPGRADE_SPECIAL_POWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char GiveOrRestoreUpgradeSpecialPower_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class GiveOrRestoreUpgradeSpecialPower : public SpecialAbilityUpdate
{
public:
	GiveOrRestoreUpgradeSpecialPower(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~GiveOrRestoreUpgradeSpecialPower();
	virtual void triggerAbilityEffect();	// override: triggerAbilityEffect slot 17 at +0x0 (0x004CD1B9)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x88;                                // +0x88; constructor store
	Int m_unk0x8C;                                 // +0x8C; constructor store
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x90
typedef char GiveOrRestoreUpgradeSpecialPower_size[(sizeof(GiveOrRestoreUpgradeSpecialPower) == 0x90) ? 1 : -1];
typedef char GiveOrRestoreUpgradeSpecialPower_off_m_unk0x88[(offsetof(GiveOrRestoreUpgradeSpecialPower, m_unk0x88) == 0x88) ? 1 : -1];
typedef char GiveOrRestoreUpgradeSpecialPower_off_m_unk0x8C[(offsetof(GiveOrRestoreUpgradeSpecialPower, m_unk0x8C) == 0x8C) ? 1 : -1];

#endif
