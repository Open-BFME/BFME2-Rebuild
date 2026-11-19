// DRAFT (h05 fpgen): LivingWorldRegionCampaign from retail's field-parse table (VA 0x00BE4120+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x5C; bases and vptr, if any, are the leading gap.
// Unresolved members: m_region (+0x0, LivingWorldRegionManager::rva00210620), m_armyPlacementPos (+0x20, INI::Rva002F592_ParseArmyPlacementPos).
#pragma once
#ifndef BFME2_LIVING_WORLD_REGION_CAMPAIGN_H
#define BFME2_LIVING_WORLD_REGION_CAMPAIGN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class LivingWorldRegionCampaign
{
public:

	UnsignedByte m_region[0x4];                            // +0x0 Region (also ConcurrentRegionBonus); unresolved: LivingWorldRegionBonusRule::ParseINI, LivingWorldRegionManager::rva00210620
	AsciiString m_regionBonusArmy;                         // +0x4 RegionBonusArmy
	AsciiString m_regionBonusResource;                     // +0x8 RegionBonusResource
	AsciiString m_regionBonusLegendary;                    // +0xC RegionBonusLegendary
	Int m_heroOnlyArmyCommandPoints;                       // +0x10 HeroOnlyArmyCommandPoints
	Int m_smallArmyCommandPoints;                          // +0x14 SmallArmyCommandPoints
	Int m_mediumArmyCommandPoints;                         // +0x18 MediumArmyCommandPoints
	Int m_armyCarryoverPoints;                             // +0x1C ArmyCarryoverPoints
	UnsignedByte m_armyPlacementPos[0x30];                 // +0x20 ArmyPlacementPos; unresolved: INI::Rva002F592_ParseArmyPlacementPos
	AsciiString m_regionConqueredSound;                    // +0x50 RegionConqueredSound
	AsciiString m_regionEffectsManagerName;                // +0x54 RegionEffectsManagerName
	Int m_armyRetreatRounds;                               // +0x58 ArmyRetreatRounds
};
typedef char LivingWorldRegionCampaign_off_m_region[(offsetof(LivingWorldRegionCampaign, m_region) == 0x0) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_regionBonusArmy[(offsetof(LivingWorldRegionCampaign, m_regionBonusArmy) == 0x4) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_regionBonusResource[(offsetof(LivingWorldRegionCampaign, m_regionBonusResource) == 0x8) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_regionBonusLegendary[(offsetof(LivingWorldRegionCampaign, m_regionBonusLegendary) == 0xC) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_heroOnlyArmyCommandPoints[(offsetof(LivingWorldRegionCampaign, m_heroOnlyArmyCommandPoints) == 0x10) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_smallArmyCommandPoints[(offsetof(LivingWorldRegionCampaign, m_smallArmyCommandPoints) == 0x14) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_mediumArmyCommandPoints[(offsetof(LivingWorldRegionCampaign, m_mediumArmyCommandPoints) == 0x18) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_armyCarryoverPoints[(offsetof(LivingWorldRegionCampaign, m_armyCarryoverPoints) == 0x1C) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_armyPlacementPos[(offsetof(LivingWorldRegionCampaign, m_armyPlacementPos) == 0x20) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_regionConqueredSound[(offsetof(LivingWorldRegionCampaign, m_regionConqueredSound) == 0x50) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_regionEffectsManagerName[(offsetof(LivingWorldRegionCampaign, m_regionEffectsManagerName) == 0x54) ? 1 : -1];
typedef char LivingWorldRegionCampaign_off_m_armyRetreatRounds[(offsetof(LivingWorldRegionCampaign, m_armyRetreatRounds) == 0x58) ? 1 : -1];

#endif
