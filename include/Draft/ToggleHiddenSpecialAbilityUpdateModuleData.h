// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BEF7E4).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ToggleHiddenSpecialAbilityUpdateModuleData.h: the INI data of ToggleHiddenSpecialAbilityUpdate. SIZE 0xCC.
// Target facts: ModuleFactory registers the module with operator new 0xCC and
// constructor RVA 0x004AE155; buildFieldParse RVA 0x0024F719 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEF7E4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_TOGGLE_HIDDEN_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_TOGGLE_HIDDEN_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char ToggleHiddenSpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class ToggleHiddenSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	ToggleHiddenSpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Bool m_showPalantirTimer;                              // +0xC8 ShowPalantirTimer
};
// SIZE 0xCC
typedef char ToggleHiddenSpecialAbilityUpdateModuleData_size[(sizeof(ToggleHiddenSpecialAbilityUpdateModuleData) == 0xCC) ? 1 : -1];
typedef char ToggleHiddenSpecialAbilityUpdateModuleData_off_m_showPalantirTimer[(offsetof(ToggleHiddenSpecialAbilityUpdateModuleData, m_showPalantirTimer) == 0xC8) ? 1 : -1];

#endif
