// ModelConditionSpecialAbilityUpdate.h: the ModelConditionSpecialAbilityUpdate module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00490D3F calls SpecialAbilityUpdate first; it stores the
// tables 0x00C4D7C8 at +0x0, 0x00C4D640 at +0xC, 0x00C4DEE8 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x00490D70 ?Rva00490D70Get@@YAHXZ
//   +0x0 slot 13 -> 0x00490F35 ?onExit@ModelConditionSpecialAbilityUpdate@@MAEX_N0@Z
//   +0x0 slot 16 -> 0x004B01E6 ?rva004B01E6@Rva004B01E6Owner@@QAE_NXZ
//   +0x0 slot 17 -> 0x00490FAE ?triggerAbilityEffect@ModelConditionSpecialAbilityUpdate@@UAEXXZ
//   +0x0 slot 22 -> 0x00490ECF ?startUnpacking@ModelConditionSpecialAbilityUpdate@@UAEXXZ
#pragma once
#ifndef BFME2_MODEL_CONDITION_SPECIAL_ABILITY_UPDATE_H
#define BFME2_MODEL_CONDITION_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char ModelConditionSpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class ModelConditionSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	ModelConditionSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void startUnpacking();	// 0x00490ECF

protected:
	virtual void onExit(bool, bool);	// 0x00490F35
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x88
typedef char ModelConditionSpecialAbilityUpdate_size[(sizeof(ModelConditionSpecialAbilityUpdate) == 0x88) ? 1 : -1];

#endif
