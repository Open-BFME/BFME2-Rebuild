// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C4D8A8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DamageFieldUpdateModuleData.h: the INI data of DamageFieldUpdate. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x00491102; buildFieldParse RVA 0x004910E7 calls FireWeaponUpdateModuleData::buildFieldParse, then adds VA 0x00C4D8A8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DAMAGE_FIELD_UPDATE_MODULE_DATA_H
#define BFME2_DAMAGE_FIELD_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/FireWeaponUpdate.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base FireWeaponUpdateModuleData: the constructor calls its constructor on this. It ends at 0x10 (its own operator-new size);
// this class's members follow it.
typedef char DamageFieldUpdateModuleData_base_size[(sizeof(FireWeaponUpdateModuleData) == 0x10) ? 1 : -1];

class DamageFieldUpdateModuleData : public FireWeaponUpdateModuleData
{
public:
	DamageFieldUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_radius;                                          // +0x10 Radius
	ObjectFilter m_objectFilter;                           // +0x14 ObjectFilter
	AsciiString m_requiredUpgrade;                         // +0x18 RequiredUpgrade
};
// SIZE 0x1C
typedef char DamageFieldUpdateModuleData_size[(sizeof(DamageFieldUpdateModuleData) == 0x1C) ? 1 : -1];
typedef char DamageFieldUpdateModuleData_off_m_radius[(offsetof(DamageFieldUpdateModuleData, m_radius) == 0x10) ? 1 : -1];
typedef char DamageFieldUpdateModuleData_off_m_objectFilter[(offsetof(DamageFieldUpdateModuleData, m_objectFilter) == 0x14) ? 1 : -1];
typedef char DamageFieldUpdateModuleData_off_m_requiredUpgrade[(offsetof(DamageFieldUpdateModuleData, m_requiredUpgrade) == 0x18) ? 1 : -1];

#endif
