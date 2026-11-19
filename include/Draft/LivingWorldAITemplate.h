// DRAFT (h05 fpgen): LivingWorldAITemplate from retail's field-parse table (VA 0x00C3BC20+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x4C; bases and vptr, if any, are the leading gap.
// Unresolved members: none.
#pragma once
#ifndef BFME2_LIVING_WORLD_A_I_TEMPLATE_H
#define BFME2_LIVING_WORLD_A_I_TEMPLATE_H

#include <stddef.h>
#include "Common/GameType.h"


class LivingWorldAITemplate
{
public:

	UnsignedByte m_unk0x0[0x4];
	Int m_desiredSoldierRatio;                             // +0x4 DesiredSoldierRatio
	Int m_desiredArcherRatio;                              // +0x8 DesiredArcherRatio
	Int m_desiredPikemenRatio;                             // +0xC DesiredPikemenRatio
	Int m_desiredCavalryRatio;                             // +0x10 DesiredCavalryRatio
	Int m_desiredMonsterRatio;                             // +0x14 DesiredMonsterRatio
	Int m_desiredHeroRatio;                                // +0x18 DesiredHeroRatio
	Int m_desiredFortressRatio;                            // +0x1C DesiredFortressRatio
	UnsignedByte m_unk0x20[0x4];
	Int m_buildingScoreArmory;                             // +0x24 BuildingScoreArmory
	Int m_buildingScoreBarracks;                           // +0x28 BuildingScoreBarracks
	Int m_buildingScoreCastle;                             // +0x2C BuildingScoreCastle
	Int m_buildingScoreFarm;                               // +0x30 BuildingScoreFarm
	Int m_bonusPreferenceResource;                         // +0x34 BonusPreferenceResource
	Int m_bonusPreferenceArmy;                             // +0x38 BonusPreferenceArmy
	Int m_bonusPreferenceLegendary;                        // +0x3C BonusPreferenceLegendary
	Int m_bonusPreferenceAttack;                           // +0x40 BonusPreferenceAttack
	Int m_bonusPreferenceDefense;                          // +0x44 BonusPreferenceDefense
	Int m_bonusPreferenceExperience;                       // +0x48 BonusPreferenceExperience
};
typedef char LivingWorldAITemplate_off_m_desiredSoldierRatio[(offsetof(LivingWorldAITemplate, m_desiredSoldierRatio) == 0x4) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_desiredArcherRatio[(offsetof(LivingWorldAITemplate, m_desiredArcherRatio) == 0x8) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_desiredPikemenRatio[(offsetof(LivingWorldAITemplate, m_desiredPikemenRatio) == 0xC) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_desiredCavalryRatio[(offsetof(LivingWorldAITemplate, m_desiredCavalryRatio) == 0x10) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_desiredMonsterRatio[(offsetof(LivingWorldAITemplate, m_desiredMonsterRatio) == 0x14) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_desiredHeroRatio[(offsetof(LivingWorldAITemplate, m_desiredHeroRatio) == 0x18) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_desiredFortressRatio[(offsetof(LivingWorldAITemplate, m_desiredFortressRatio) == 0x1C) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_buildingScoreArmory[(offsetof(LivingWorldAITemplate, m_buildingScoreArmory) == 0x24) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_buildingScoreBarracks[(offsetof(LivingWorldAITemplate, m_buildingScoreBarracks) == 0x28) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_buildingScoreCastle[(offsetof(LivingWorldAITemplate, m_buildingScoreCastle) == 0x2C) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_buildingScoreFarm[(offsetof(LivingWorldAITemplate, m_buildingScoreFarm) == 0x30) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_bonusPreferenceResource[(offsetof(LivingWorldAITemplate, m_bonusPreferenceResource) == 0x34) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_bonusPreferenceArmy[(offsetof(LivingWorldAITemplate, m_bonusPreferenceArmy) == 0x38) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_bonusPreferenceLegendary[(offsetof(LivingWorldAITemplate, m_bonusPreferenceLegendary) == 0x3C) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_bonusPreferenceAttack[(offsetof(LivingWorldAITemplate, m_bonusPreferenceAttack) == 0x40) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_bonusPreferenceDefense[(offsetof(LivingWorldAITemplate, m_bonusPreferenceDefense) == 0x44) ? 1 : -1];
typedef char LivingWorldAITemplate_off_m_bonusPreferenceExperience[(offsetof(LivingWorldAITemplate, m_bonusPreferenceExperience) == 0x48) ? 1 : -1];

#endif
