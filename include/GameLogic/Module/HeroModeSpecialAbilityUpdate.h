// HeroModeSpecialAbilityUpdate.h: the HeroModeSpecialAbilityUpdate module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00492179 calls SpecialAbilityUpdate first; it stores the
// tables 0x00C4DEF8 at +0x0, 0x00C4D640 at +0xC, 0x00C4DEE8 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004921AA ?Rva004921AAGet@@YAHXZ
//   +0x0 slot 17 -> 0x00492329 ?triggerAbilityEffect@Rva00492179@@UAEXXZ
//   +0x0 slot 18 -> 0x0049226C ?rva0049226C@Rva00492179@@UAEXXZ
//   +0x0 slot 22 -> 0x00492231 ?startUnpacking@Rva00492179@@UAEXXZ
#pragma once
#ifndef BFME2_HERO_MODE_SPECIAL_ABILITY_UPDATE_H
#define BFME2_HERO_MODE_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char HeroModeSpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class HeroModeSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	HeroModeSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// 0x00492329
	virtual void startUnpacking();	// 0x00492231
	virtual void vslot18();	// 0x0049226C: the effect ends

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x88
typedef char HeroModeSpecialAbilityUpdate_size[(sizeof(HeroModeSpecialAbilityUpdate) == 0x88) ? 1 : -1];

#endif
