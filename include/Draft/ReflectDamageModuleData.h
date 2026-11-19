// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C59C70).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ReflectDamageModuleData.h: the INI data of ReflectDamage. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x004BAB9A; buildFieldParse RVA 0x004BAB25 adds VA 0x00C59C70 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_REFLECT_DAMAGE_MODULE_DATA_H
#define BFME2_REFLECT_DAMAGE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Damage.h"

class INI;
class MultiIniFieldParse;

class ReflectDamageModuleData : public ModuleData
{
public:
	ReflectDamageModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	DamageTypeFlags m_damageTypesToReflect;                // +0x8 DamageTypesToReflect
	Real m_reflectDamagePercentage;                        // +0xC ReflectDamagePercentage; percent
	Real m_minimumDamageToReflect;                         // +0x10 MinimumDamageToReflect
};
// SIZE 0x14
typedef char ReflectDamageModuleData_size[(sizeof(ReflectDamageModuleData) == 0x14) ? 1 : -1];
typedef char ReflectDamageModuleData_off_m_damageTypesToReflect[(offsetof(ReflectDamageModuleData, m_damageTypesToReflect) == 0x8) ? 1 : -1];
typedef char ReflectDamageModuleData_off_m_reflectDamagePercentage[(offsetof(ReflectDamageModuleData, m_reflectDamagePercentage) == 0xC) ? 1 : -1];
typedef char ReflectDamageModuleData_off_m_minimumDamageToReflect[(offsetof(ReflectDamageModuleData, m_minimumDamageToReflect) == 0x10) ? 1 : -1];

#endif
