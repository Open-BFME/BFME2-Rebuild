// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C6BB18, 0x00BF0620).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// PillageModuleData.h: the INI data of PillageModule. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x00253830; buildFieldParse RVA 0x00253869 adds VA 0x00C6BB18 at +0x0, VA 0x00BF0620 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_PILLAGE_MODULE_DATA_H
#define BFME2_PILLAGE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class PillageModuleData : public ModuleData
{
public:
	PillageModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_pillageAmount;                           // +0x8 PillageAmount
	UnsignedInt m_numDamageEventsPerPillage;               // +0xC NumDamageEventsPerPillage
	ObjectFilter m_pillageFilter;                          // +0x10 PillageFilter
};
// SIZE 0x14
typedef char PillageModuleData_size[(sizeof(PillageModuleData) == 0x14) ? 1 : -1];
typedef char PillageModuleData_off_m_pillageAmount[(offsetof(PillageModuleData, m_pillageAmount) == 0x8) ? 1 : -1];
typedef char PillageModuleData_off_m_numDamageEventsPerPillage[(offsetof(PillageModuleData, m_numDamageEventsPerPillage) == 0xC) ? 1 : -1];
typedef char PillageModuleData_off_m_pillageFilter[(offsetof(PillageModuleData, m_pillageFilter) == 0x10) ? 1 : -1];

#endif
