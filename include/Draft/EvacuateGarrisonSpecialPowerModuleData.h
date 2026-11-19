// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C6BB18).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// EvacuateGarrisonSpecialPowerModuleData.h: the INI data of EvacuateGarrisonSpecialPower. SIZE 0xC8.
// Target facts: ModuleFactory registers the module with operator new 0xC8 and
// constructor RVA 0x004CDFD9; buildFieldParse RVA 0x004CDBE1 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C6BB18 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_EVACUATE_GARRISON_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_EVACUATE_GARRISON_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char EvacuateGarrisonSpecialPowerModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class EvacuateGarrisonSpecialPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	EvacuateGarrisonSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

};
// SIZE 0xC8
typedef char EvacuateGarrisonSpecialPowerModuleData_size[(sizeof(EvacuateGarrisonSpecialPowerModuleData) == 0xC8) ? 1 : -1];

#endif
