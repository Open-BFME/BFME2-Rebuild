// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5A4A8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// AODCrushCollideModuleData.h: the INI data of AODCrushCollide. SIZE 0x48.
// Target facts: ModuleFactory registers the module with operator new 0x48 and
// constructor RVA 0x004BBF89; buildFieldParse RVA 0x004BBFFC adds VA 0x00C5A4A8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_A_O_D_CRUSH_COLLIDE_MODULE_DATA_H
#define BFME2_A_O_D_CRUSH_COLLIDE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;

class AODCrushCollideModuleData : public ModuleData
{
public:
	AODCrushCollideModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const FXList * m_smallFXList;                          // +0x8 SmallFXList
	const ObjectCreationList * m_smallObjectCreationList;  // +0xC SmallObjectCreationList
	const FXList * m_mediumFXList;                         // +0x10 MediumFXList
	const ObjectCreationList * m_mediumObjectCreationList; // +0x14 MediumObjectCreationList
	const FXList * m_largeFXList;                          // +0x18 LargeFXList
	const ObjectCreationList * m_largeObjectCreationList;  // +0x1C LargeObjectCreationList
	Int m_damageType;                                      // +0x20 DamageType; index into a name list (enum)
	Int m_deathType;                                       // +0x24 DeathType; index into a name list (enum)
	Real m_damage;                                         // +0x28 Damage
	ObjectFilter m_specialObject;                          // +0x2C SpecialObject
	Int m_specialDamageType;                               // +0x30 SpecialDamageType; index into a name list (enum)
	Int m_specialDeathType;                                // +0x34 SpecialDeathType; index into a name list (enum)
	Real m_specialDamage;                                  // +0x38 SpecialDamage
	Int m_selfDamageType;                                  // +0x3C SelfDamageType; index into a name list (enum)
	Int m_selfDeathType;                                   // +0x40 SelfDeathType; index into a name list (enum)
	Real m_selfDamage;                                     // +0x44 SelfDamage
};
// SIZE 0x48
typedef char AODCrushCollideModuleData_size[(sizeof(AODCrushCollideModuleData) == 0x48) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_smallFXList[(offsetof(AODCrushCollideModuleData, m_smallFXList) == 0x8) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_smallObjectCreationList[(offsetof(AODCrushCollideModuleData, m_smallObjectCreationList) == 0xC) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_mediumFXList[(offsetof(AODCrushCollideModuleData, m_mediumFXList) == 0x10) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_mediumObjectCreationList[(offsetof(AODCrushCollideModuleData, m_mediumObjectCreationList) == 0x14) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_largeFXList[(offsetof(AODCrushCollideModuleData, m_largeFXList) == 0x18) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_largeObjectCreationList[(offsetof(AODCrushCollideModuleData, m_largeObjectCreationList) == 0x1C) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_damageType[(offsetof(AODCrushCollideModuleData, m_damageType) == 0x20) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_deathType[(offsetof(AODCrushCollideModuleData, m_deathType) == 0x24) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_damage[(offsetof(AODCrushCollideModuleData, m_damage) == 0x28) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_specialObject[(offsetof(AODCrushCollideModuleData, m_specialObject) == 0x2C) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_specialDamageType[(offsetof(AODCrushCollideModuleData, m_specialDamageType) == 0x30) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_specialDeathType[(offsetof(AODCrushCollideModuleData, m_specialDeathType) == 0x34) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_specialDamage[(offsetof(AODCrushCollideModuleData, m_specialDamage) == 0x38) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_selfDamageType[(offsetof(AODCrushCollideModuleData, m_selfDamageType) == 0x3C) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_selfDeathType[(offsetof(AODCrushCollideModuleData, m_selfDeathType) == 0x40) ? 1 : -1];
typedef char AODCrushCollideModuleData_off_m_selfDamage[(offsetof(AODCrushCollideModuleData, m_selfDamage) == 0x44) ? 1 : -1];

#endif
