// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C6BB18, 0x00C59F70).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// CallHelpOnDamageModuleData.h: the INI data of CallHelpOnDamage. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x004BB3A7; buildFieldParse RVA 0x004BB41B adds VA 0x00C6BB18 at +0x0, VA 0x00C59F70 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CALL_HELP_ON_DAMAGE_MODULE_DATA_H
#define BFME2_CALL_HELP_ON_DAMAGE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Damage.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class CallHelpOnDamageModuleData : public ModuleData
{
public:
	CallHelpOnDamageModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	DamageTypeFlags m_damageTypes;                         // +0x8 DamageTypes
	Real m_callRadius;                                     // +0xC CallRadius
	UnsignedInt m_callDelay;                               // +0x10 CallDelay; duration, frames
	Bool m_moveToAttacker;                                 // +0x14 MoveToAttacker
	ObjectFilter m_validObjects;                           // +0x18 ValidObjects
};
// SIZE 0x1C
typedef char CallHelpOnDamageModuleData_size[(sizeof(CallHelpOnDamageModuleData) == 0x1C) ? 1 : -1];
typedef char CallHelpOnDamageModuleData_off_m_damageTypes[(offsetof(CallHelpOnDamageModuleData, m_damageTypes) == 0x8) ? 1 : -1];
typedef char CallHelpOnDamageModuleData_off_m_callRadius[(offsetof(CallHelpOnDamageModuleData, m_callRadius) == 0xC) ? 1 : -1];
typedef char CallHelpOnDamageModuleData_off_m_callDelay[(offsetof(CallHelpOnDamageModuleData, m_callDelay) == 0x10) ? 1 : -1];
typedef char CallHelpOnDamageModuleData_off_m_moveToAttacker[(offsetof(CallHelpOnDamageModuleData, m_moveToAttacker) == 0x14) ? 1 : -1];
typedef char CallHelpOnDamageModuleData_off_m_validObjects[(offsetof(CallHelpOnDamageModuleData, m_validObjects) == 0x18) ? 1 : -1];

#endif
