// DRAFT (h05 fpgen): LivingWorldPlayerArmy from retail's field-parse table (VA 0x00C39538+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x68; bases and vptr, if any, are the leading gap.
// Unresolved members: m_armyEntry (+0x0, ArmySummary::parseArmyEntry), m_color (+0x24, INI::parseRGBColor), m_nightColor (+0x28, INI::parseRGBColor).
#pragma once
#ifndef BFME2_LIVING_WORLD_PLAYER_ARMY_H
#define BFME2_LIVING_WORLD_PLAYER_ARMY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class LivingWorldPlayerArmy
{
public:

	UnsignedByte m_armyEntry[0x18];                        // +0x0 ArmyEntry; unresolved: ArmySummary::parseArmyEntry
	AsciiString m_name;                                    // +0x18 Name
	UnsignedByte m_unk0x1C[0x8];
	UnsignedByte m_color[0x4];                             // +0x24 Color; unresolved: INI::parseRGBColor
	UnsignedByte m_nightColor[0x38];                       // +0x28 NightColor; unresolved: INI::parseRGBColor
	Int m_survivalThreshhold;                              // +0x60 SurvivalThreshhold
	AsciiString m_displayNameTag;                          // +0x64 DisplayNameTag
};
typedef char LivingWorldPlayerArmy_off_m_armyEntry[(offsetof(LivingWorldPlayerArmy, m_armyEntry) == 0x0) ? 1 : -1];
typedef char LivingWorldPlayerArmy_off_m_name[(offsetof(LivingWorldPlayerArmy, m_name) == 0x18) ? 1 : -1];
typedef char LivingWorldPlayerArmy_off_m_color[(offsetof(LivingWorldPlayerArmy, m_color) == 0x24) ? 1 : -1];
typedef char LivingWorldPlayerArmy_off_m_nightColor[(offsetof(LivingWorldPlayerArmy, m_nightColor) == 0x28) ? 1 : -1];
typedef char LivingWorldPlayerArmy_off_m_survivalThreshhold[(offsetof(LivingWorldPlayerArmy, m_survivalThreshhold) == 0x60) ? 1 : -1];
typedef char LivingWorldPlayerArmy_off_m_displayNameTag[(offsetof(LivingWorldPlayerArmy, m_displayNameTag) == 0x64) ? 1 : -1];

#endif
