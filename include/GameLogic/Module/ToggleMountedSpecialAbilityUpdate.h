// ToggleMountedSpecialAbilityUpdate.h: the ToggleMountedSpecialAbilityUpdate module. SIZE 0x90 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004ADAB5 calls SpecialAbilityUpdate first; it stores the
// tables 0x00C551C0 at +0x0, 0x00C562D8 at +0xC, 0x00C4DEE8 at +0x10, 0x00C55198 at +0x20. Members are what xfer (0x004ADA2D) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004AD9DA ?name@Rva004AD9DANamed@@QBEPBDXZ
//   +0x0 slot 13 -> 0x004ADBF2 ?Rva004ADBF2@Rva004AD9B0@@MAEX_N0@Z
//   +0x0 slot 16 -> 0x004B01E6 ?rva004B01E6@Rva004B01E6Owner@@QAE_NXZ
//   +0x0 slot 17 -> 0x004ADF0B ?triggerAbilityEffect@ToggleMountedSpecialAbilityUpdate@@UAEXXZ
//   +0x0 slot 19 -> 0x004ADC88 ?finishAbility@Rva004ADC88@@QAEXXZ
//   +0x0 slot 20 -> 0x004ADB1B ?handlePackingProcessing@Rva004AD9B0@@UAE_NXZ
//   +0xC slot 3 -> 0x004B01D9 ?getRailedTransportDockUpdateInterface@RailedTransportDockUpdate@@UAEPAVRailedTransportDockUpdateInterface@@XZ
//   +0x20 slot 0 -> 0x004ADC38 ?rva004ADC38@Rva004ADC38@@QAEXHHHHH@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@SpecialAbilityUpdate@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_TOGGLE_MOUNTED_SPECIAL_ABILITY_UPDATE_H
#define BFME2_TOGGLE_MOUNTED_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char ToggleMountedSpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class ToggleMountedSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	ToggleMountedSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual void triggerAbilityEffect();	// override: triggerAbilityEffect slot 17 at +0x0 (0x004ADF0B)
	virtual void finishAbility();	// slot 19, 0x004ADC88
	void removeAndDestroyObject();	// 0x004AD9E0 (map: placeholder rva004AD9E0): the unit leaves its group and the pathfind map, its drawable is hidden, and it is destroyed

protected:
	virtual void onExit(bool, bool);	// 0x004ADBF2
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x88[0x4];
	Bool m_unk0x8C;                                // +0x8C; xfer Bool
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x90
typedef char ToggleMountedSpecialAbilityUpdate_size[(sizeof(ToggleMountedSpecialAbilityUpdate) == 0x90) ? 1 : -1];
typedef char ToggleMountedSpecialAbilityUpdate_off_m_unk0x8C[(offsetof(ToggleMountedSpecialAbilityUpdate, m_unk0x8C) == 0x8C) ? 1 : -1];

#endif
