// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C41690).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_query.
// AutoAbilityBehaviorModuleData.h: the INI data of AutoAbilityBehavior. SIZE 0x60.
// Target facts: ModuleFactory registers the module with operator new 0x60 and
// constructor RVA 0x0045A2E7; buildFieldParse RVA 0x0045A36E adds VA 0x00C41690 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_query +0x2C: INI::Rva0045A22E_ParseQuery 0x0045A22E
#pragma once
#ifndef BFME2_AUTO_ABILITY_BEHAVIOR_MODULE_DATA_H
#define BFME2_AUTO_ABILITY_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;

class AutoAbilityBehaviorModuleData : public ModuleData
{
public:
	AutoAbilityBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_maxScanRange;                                   // +0x8 MaxScanRange
	Real m_minScanRange;                                   // +0xC MinScanRange
	Real m_workingRadius;                                  // +0x10 WorkingRadius
	Real m_idleTimeSeconds;                                // +0x14 IdleTimeSeconds
	AsciiString m_specialAbility;                          // +0x18 SpecialAbility
	ObjectStatusMaskType m_forbiddenStatus;                // +0x1C ForbiddenStatus; BitFlags<101>
	UnsignedByte m_query[0x30];                            // +0x2C Query; unresolved: INI::Rva0045A22E_ParseQuery
	Bool m_startsActive;                                   // +0x5C StartsActive
	Bool m_baseMaxRangeFromStartPos;                       // +0x5D BaseMaxRangeFromStartPos
	Bool m_adjustAttackMeleePosition;                      // +0x5E AdjustAttackMeleePosition
	Bool m_allowSelf;                                      // +0x5F AllowSelf
};
// SIZE 0x60
typedef char AutoAbilityBehaviorModuleData_size[(sizeof(AutoAbilityBehaviorModuleData) == 0x60) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_maxScanRange[(offsetof(AutoAbilityBehaviorModuleData, m_maxScanRange) == 0x8) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_minScanRange[(offsetof(AutoAbilityBehaviorModuleData, m_minScanRange) == 0xC) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_workingRadius[(offsetof(AutoAbilityBehaviorModuleData, m_workingRadius) == 0x10) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_idleTimeSeconds[(offsetof(AutoAbilityBehaviorModuleData, m_idleTimeSeconds) == 0x14) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_specialAbility[(offsetof(AutoAbilityBehaviorModuleData, m_specialAbility) == 0x18) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_forbiddenStatus[(offsetof(AutoAbilityBehaviorModuleData, m_forbiddenStatus) == 0x1C) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_query[(offsetof(AutoAbilityBehaviorModuleData, m_query) == 0x2C) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_startsActive[(offsetof(AutoAbilityBehaviorModuleData, m_startsActive) == 0x5C) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_baseMaxRangeFromStartPos[(offsetof(AutoAbilityBehaviorModuleData, m_baseMaxRangeFromStartPos) == 0x5D) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_adjustAttackMeleePosition[(offsetof(AutoAbilityBehaviorModuleData, m_adjustAttackMeleePosition) == 0x5E) ? 1 : -1];
typedef char AutoAbilityBehaviorModuleData_off_m_allowSelf[(offsetof(AutoAbilityBehaviorModuleData, m_allowSelf) == 0x5F) ? 1 : -1];

#endif
