// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5F638).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DominateEnemySpecialPowerModuleData.h: the INI data of DominateEnemySpecialPower. SIZE 0xDC.
// Target facts: ModuleFactory registers the module with operator new 0xDC and
// constructor RVA 0x004CCA24; buildFieldParse RVA 0x004CC96D calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C5F638 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DOMINATE_ENEMY_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_DOMINATE_ENEMY_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char DominateEnemySpecialPowerModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class DominateEnemySpecialPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	DominateEnemySpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_dominateRadius;                                 // +0xC8 DominateRadius
	const FXList * m_triggerFX;                            // +0xCC TriggerFX
	const FXList * m_dominatedFX;                          // +0xD0 DominatedFX
	Bool m_permanentlyConvert;                             // +0xD4 PermanentlyConvert
	ObjectFilter m_attributeModifierAffects;               // +0xD8 AttributeModifierAffects
};
// SIZE 0xDC
typedef char DominateEnemySpecialPowerModuleData_size[(sizeof(DominateEnemySpecialPowerModuleData) == 0xDC) ? 1 : -1];
typedef char DominateEnemySpecialPowerModuleData_off_m_dominateRadius[(offsetof(DominateEnemySpecialPowerModuleData, m_dominateRadius) == 0xC8) ? 1 : -1];
typedef char DominateEnemySpecialPowerModuleData_off_m_triggerFX[(offsetof(DominateEnemySpecialPowerModuleData, m_triggerFX) == 0xCC) ? 1 : -1];
typedef char DominateEnemySpecialPowerModuleData_off_m_dominatedFX[(offsetof(DominateEnemySpecialPowerModuleData, m_dominatedFX) == 0xD0) ? 1 : -1];
typedef char DominateEnemySpecialPowerModuleData_off_m_permanentlyConvert[(offsetof(DominateEnemySpecialPowerModuleData, m_permanentlyConvert) == 0xD4) ? 1 : -1];
typedef char DominateEnemySpecialPowerModuleData_off_m_attributeModifierAffects[(offsetof(DominateEnemySpecialPowerModuleData, m_attributeModifierAffects) == 0xD8) ? 1 : -1];

#endif
