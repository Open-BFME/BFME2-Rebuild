// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5F810).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// CurseSpecialPowerModuleData.h: the INI data of CurseSpecialPower. SIZE 0xD4.
// Target facts: ModuleFactory registers the module with operator new 0xD4 and
// constructor RVA 0x004CCD34; buildFieldParse RVA 0x004CCD64 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C5F810 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CURSE_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_CURSE_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char CurseSpecialPowerModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class CurseSpecialPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	CurseSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const FXList * m_triggerFX;                            // +0xC8 TriggerFX
	const FXList * m_cursedFX;                             // +0xCC CursedFX
	Real m_cursePercentage;                                // +0xD0 CursePercentage; percent
};
// SIZE 0xD4
typedef char CurseSpecialPowerModuleData_size[(sizeof(CurseSpecialPowerModuleData) == 0xD4) ? 1 : -1];
typedef char CurseSpecialPowerModuleData_off_m_triggerFX[(offsetof(CurseSpecialPowerModuleData, m_triggerFX) == 0xC8) ? 1 : -1];
typedef char CurseSpecialPowerModuleData_off_m_cursedFX[(offsetof(CurseSpecialPowerModuleData, m_cursedFX) == 0xCC) ? 1 : -1];
typedef char CurseSpecialPowerModuleData_off_m_cursePercentage[(offsetof(CurseSpecialPowerModuleData, m_cursePercentage) == 0xD0) ? 1 : -1];

#endif
