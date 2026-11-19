// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5E6E4).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// CombineHordeSpecialPowerModuleData.h: the INI data of CombineHordeSpecialPower. SIZE 0x80.
// Target facts: ModuleFactory registers the module with operator new 0x80 and
// constructor RVA 0x004C886E; buildFieldParse RVA 0x004C87A9 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5E6E4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_COMBINE_HORDE_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_COMBINE_HORDE_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char CombineHordeSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class CombineHordeSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	CombineHordeSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_scanRange;                                      // +0x7C ScanRange
};
// SIZE 0x80
typedef char CombineHordeSpecialPowerModuleData_size[(sizeof(CombineHordeSpecialPowerModuleData) == 0x80) ? 1 : -1];
typedef char CombineHordeSpecialPowerModuleData_off_m_scanRange[(offsetof(CombineHordeSpecialPowerModuleData, m_scanRange) == 0x7C) ? 1 : -1];

#endif
