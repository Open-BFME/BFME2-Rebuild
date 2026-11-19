// DRAFT (h05 fpgen): SkirmishAIData from retail's field-parse table (VA 0x00BFDA50+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x878; bases and vptr, if any, are the leading gap.
// Unresolved members: m_combatChainDefinition (+0x0, INI::Rva002A8945_ParseCombatChainDefinition), m_anyTypeTemplateDisabledSlots (+0x854, INI::Rva004E8EFF_ParseDisabledSlots).
#pragma once
#ifndef BFME2_SKIRMISH_A_I_DATA_H
#define BFME2_SKIRMISH_A_I_DATA_H

#include <stddef.h>
#include "Common/GameType.h"


class SkirmishAIData
{
public:

	UnsignedByte m_combatChainDefinition[0x848];           // +0x0 CombatChainDefinition (also BrutalDifficultyCheats, DifficultyTuning); unresolved: INI::Rva002A8945_ParseCombatChainDefinition, INI::Rva002A898A_ParseBrutalDifficultyCheats, INI::Rva002A89D2_ParseDifficultyTuning
	Bool m_disableBaseBuilding;                            // +0x848 DisableBaseBuilding
	Bool m_disableEconomyBuilding;                         // +0x849 DisableEconomyBuilding
	Bool m_disableUnitBuilding;                            // +0x84A DisableUnitBuilding
	Bool m_disableScienceUpgrading;                        // +0x84B DisableScienceUpgrading
	Bool m_disableUnitUpgrading;                           // +0x84C DisableUnitUpgrading
	Bool m_disableTacticalAI;                              // +0x84D DisableTacticalAI
	Bool m_disableTeamBuilding;                            // +0x84E DisableTeamBuilding
	Bool m_disableWallBuilding;                            // +0x84F DisableWallBuilding
	Bool m_makeAllSkirmishSidesAIControlled;               // +0x850 MakeAllSkirmishSidesAIControlled
	UnsignedByte m_unk0x851[0x3];
	UnsignedByte m_anyTypeTemplateDisabledSlots[0xC];      // +0x854 AnyTypeTemplateDisabledSlots; unresolved: INI::Rva004E8EFF_ParseDisabledSlots
	Real m_teamIdleCheckRadius;                            // +0x860 TeamIdleCheckRadius
	Real m_teamTimeUntilConsideredIdle;                    // +0x864 TeamTimeUntilConsideredIdle
	Real m_defenseTreeNodeRadius;                          // +0x868 DefenseTreeNodeRadius
	Real m_timeBetweenEnemyChangeLO;                       // +0x86C TimeBetweenEnemyChangeLO
	Real m_timeBetweenEnemyChangeHI;                       // +0x870 TimeBetweenEnemyChangeHI
	Real m_defaultTargetThreatRadius;                      // +0x874 DefaultTargetThreatRadius
};
typedef char SkirmishAIData_off_m_combatChainDefinition[(offsetof(SkirmishAIData, m_combatChainDefinition) == 0x0) ? 1 : -1];
typedef char SkirmishAIData_off_m_disableBaseBuilding[(offsetof(SkirmishAIData, m_disableBaseBuilding) == 0x848) ? 1 : -1];
typedef char SkirmishAIData_off_m_disableEconomyBuilding[(offsetof(SkirmishAIData, m_disableEconomyBuilding) == 0x849) ? 1 : -1];
typedef char SkirmishAIData_off_m_disableUnitBuilding[(offsetof(SkirmishAIData, m_disableUnitBuilding) == 0x84A) ? 1 : -1];
typedef char SkirmishAIData_off_m_disableScienceUpgrading[(offsetof(SkirmishAIData, m_disableScienceUpgrading) == 0x84B) ? 1 : -1];
typedef char SkirmishAIData_off_m_disableUnitUpgrading[(offsetof(SkirmishAIData, m_disableUnitUpgrading) == 0x84C) ? 1 : -1];
typedef char SkirmishAIData_off_m_disableTacticalAI[(offsetof(SkirmishAIData, m_disableTacticalAI) == 0x84D) ? 1 : -1];
typedef char SkirmishAIData_off_m_disableTeamBuilding[(offsetof(SkirmishAIData, m_disableTeamBuilding) == 0x84E) ? 1 : -1];
typedef char SkirmishAIData_off_m_disableWallBuilding[(offsetof(SkirmishAIData, m_disableWallBuilding) == 0x84F) ? 1 : -1];
typedef char SkirmishAIData_off_m_makeAllSkirmishSidesAIControlled[(offsetof(SkirmishAIData, m_makeAllSkirmishSidesAIControlled) == 0x850) ? 1 : -1];
typedef char SkirmishAIData_off_m_anyTypeTemplateDisabledSlots[(offsetof(SkirmishAIData, m_anyTypeTemplateDisabledSlots) == 0x854) ? 1 : -1];
typedef char SkirmishAIData_off_m_teamIdleCheckRadius[(offsetof(SkirmishAIData, m_teamIdleCheckRadius) == 0x860) ? 1 : -1];
typedef char SkirmishAIData_off_m_teamTimeUntilConsideredIdle[(offsetof(SkirmishAIData, m_teamTimeUntilConsideredIdle) == 0x864) ? 1 : -1];
typedef char SkirmishAIData_off_m_defenseTreeNodeRadius[(offsetof(SkirmishAIData, m_defenseTreeNodeRadius) == 0x868) ? 1 : -1];
typedef char SkirmishAIData_off_m_timeBetweenEnemyChangeLO[(offsetof(SkirmishAIData, m_timeBetweenEnemyChangeLO) == 0x86C) ? 1 : -1];
typedef char SkirmishAIData_off_m_timeBetweenEnemyChangeHI[(offsetof(SkirmishAIData, m_timeBetweenEnemyChangeHI) == 0x870) ? 1 : -1];
typedef char SkirmishAIData_off_m_defaultTargetThreatRadius[(offsetof(SkirmishAIData, m_defaultTargetThreatRadius) == 0x874) ? 1 : -1];

#endif
