// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5FB9C).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// StoreObjectsSpecialPowerModuleData.h: the INI data of StoreObjectsSpecialPower. SIZE 0xCC.
// Target facts: ModuleFactory registers the module with operator new 0xCC and
// constructor RVA 0x004CD8FB; buildFieldParse RVA 0x004CD918 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C5FB9C at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_STORE_OBJECTS_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_STORE_OBJECTS_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char StoreObjectsSpecialPowerModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class StoreObjectsSpecialPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	StoreObjectsSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_radius;                                         // +0xC8 Radius
};
// SIZE 0xCC
typedef char StoreObjectsSpecialPowerModuleData_size[(sizeof(StoreObjectsSpecialPowerModuleData) == 0xCC) ? 1 : -1];
typedef char StoreObjectsSpecialPowerModuleData_off_m_radius[(offsetof(StoreObjectsSpecialPowerModuleData, m_radius) == 0xC8) ? 1 : -1];

#endif
