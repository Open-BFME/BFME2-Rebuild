// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C4ABE0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DamageFilteredCreateObjectDieModuleData.h: the INI data of DamageFilteredCreateObjectDie. SIZE 0x48.
// Target facts: ModuleFactory registers the module with operator new 0x48 and
// constructor RVA 0x00485F42; buildFieldParse RVA 0x00485F65 adds VA 0x00C4ABE0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DAMAGE_FILTERED_CREATE_OBJECT_DIE_MODULE_DATA_H
#define BFME2_DAMAGE_FILTERED_CREATE_OBJECT_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"

class INI;
class MultiIniFieldParse;
class ObjectCreationList;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char DamageFilteredCreateObjectDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class DamageFilteredCreateObjectDieModuleData : public DestroyDieModuleData
{
public:
	DamageFilteredCreateObjectDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	ObjectCreationList * m_creationList;             // +0x38 CreationList
	Int m_damageTypeTriggersInstantly;                     // +0x3C DamageTypeTriggersInstantly; index into a name list (enum)
	Int m_damageTypeTriggersForDuration;                   // +0x40 DamageTypeTriggersForDuration; index into a name list (enum)
	UnsignedInt m_postFilterTriggeredDuration;             // +0x44 PostFilterTriggeredDuration; duration, frames
};
// SIZE 0x48
typedef char DamageFilteredCreateObjectDieModuleData_size[(sizeof(DamageFilteredCreateObjectDieModuleData) == 0x48) ? 1 : -1];
typedef char DamageFilteredCreateObjectDieModuleData_off_m_creationList[(offsetof(DamageFilteredCreateObjectDieModuleData, m_creationList) == 0x38) ? 1 : -1];
typedef char DamageFilteredCreateObjectDieModuleData_off_m_damageTypeTriggersInstantly[(offsetof(DamageFilteredCreateObjectDieModuleData, m_damageTypeTriggersInstantly) == 0x3C) ? 1 : -1];
typedef char DamageFilteredCreateObjectDieModuleData_off_m_damageTypeTriggersForDuration[(offsetof(DamageFilteredCreateObjectDieModuleData, m_damageTypeTriggersForDuration) == 0x40) ? 1 : -1];
typedef char DamageFilteredCreateObjectDieModuleData_off_m_postFilterTriggeredDuration[(offsetof(DamageFilteredCreateObjectDieModuleData, m_postFilterTriggeredDuration) == 0x44) ? 1 : -1];

#endif
