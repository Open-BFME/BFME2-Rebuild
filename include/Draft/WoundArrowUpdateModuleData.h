// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5DEAC).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// WoundArrowUpdateModuleData.h: the INI data of WoundArrowUpdate. SIZE 0xD0.
// Target facts: ModuleFactory registers the module with operator new 0xD0 and
// constructor RVA 0x004C68C1; buildFieldParse RVA 0x004C68EA calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C5DEAC at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_WOUND_ARROW_UPDATE_MODULE_DATA_H
#define BFME2_WOUND_ARROW_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char WoundArrowUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class WoundArrowUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	WoundArrowUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_fleeDistance;                                   // +0xC8 FleeDistance
	UnsignedInt m_forbiddenConditions;                     // +0xCC ForbiddenConditions; bit string
};
// SIZE 0xD0
typedef char WoundArrowUpdateModuleData_size[(sizeof(WoundArrowUpdateModuleData) == 0xD0) ? 1 : -1];
typedef char WoundArrowUpdateModuleData_off_m_fleeDistance[(offsetof(WoundArrowUpdateModuleData, m_fleeDistance) == 0xC8) ? 1 : -1];
typedef char WoundArrowUpdateModuleData_off_m_forbiddenConditions[(offsetof(WoundArrowUpdateModuleData, m_forbiddenConditions) == 0xCC) ? 1 : -1];

#endif
