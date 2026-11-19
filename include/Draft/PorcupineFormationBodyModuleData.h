// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5C2D8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// PorcupineFormationBodyModuleData.h: the INI data of PorcupineFormationBodyModule. SIZE 0x70.
// Target facts: ModuleFactory registers the module with operator new 0x70 and
// constructor RVA 0x004C225D; buildFieldParse RVA 0x004C1FCB calls ActiveBodyModuleData::buildFieldParse, then adds VA 0x00C5C2D8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_PORCUPINE_FORMATION_BODY_MODULE_DATA_H
#define BFME2_PORCUPINE_FORMATION_BODY_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBodyModuleData.h"

class INI;
class MultiIniFieldParse;
class WeaponTemplate;

// Base ActiveBodyModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char PorcupineFormationBodyModuleData_base_size[(sizeof(ActiveBodyModuleData) == 0x64) ? 1 : -1];

class PorcupineFormationBodyModuleData : public ActiveBodyModuleData
{
public:
	PorcupineFormationBodyModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const WeaponTemplate * m_damageWeaponTemplate;         // +0x64 DamageWeaponTemplate
	const WeaponTemplate * m_crushDamageWeaponTemplate;    // +0x68 CrushDamageWeaponTemplate
	Byte m_crusherLevelResisted;                           // +0x6C CrusherLevelResisted
};
// SIZE 0x70
typedef char PorcupineFormationBodyModuleData_size[(sizeof(PorcupineFormationBodyModuleData) == 0x70) ? 1 : -1];
typedef char PorcupineFormationBodyModuleData_off_m_damageWeaponTemplate[(offsetof(PorcupineFormationBodyModuleData, m_damageWeaponTemplate) == 0x64) ? 1 : -1];
typedef char PorcupineFormationBodyModuleData_off_m_crushDamageWeaponTemplate[(offsetof(PorcupineFormationBodyModuleData, m_crushDamageWeaponTemplate) == 0x68) ? 1 : -1];
typedef char PorcupineFormationBodyModuleData_off_m_crusherLevelResisted[(offsetof(PorcupineFormationBodyModuleData, m_crusherLevelResisted) == 0x6C) ? 1 : -1];

#endif
