// DRAFT (h05 fpgen): Fire from retail's field-parse table (VA 0x00C08708+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x40; bases and vptr, if any, are the leading gap.
// Unresolved members: m_burntTerrainColor (+0x8, INI::parseRGBColor), m_fuelIndicatorColor (+0x14, INI::parseRGBColor), m_scorchFrequency (+0x24, INI::parseGameClientRandomVariable), m_scorchSize (+0x30, INI::parseGameClientRandomVariable).
#pragma once
#ifndef BFME2_FIRE_H
#define BFME2_FIRE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class Fire
{
public:

	AsciiString m_terrainFireSystem;                       // +0x0 TerrainFireSystem
	AsciiString m_terrainSmokeSystem;                      // +0x4 TerrainSmokeSystem
	UnsignedByte m_burntTerrainColor[0xC];                 // +0x8 BurntTerrainColor; unresolved: INI::parseRGBColor
	UnsignedByte m_fuelIndicatorColor[0xC];                // +0x14 FuelIndicatorColor; unresolved: INI::parseRGBColor
	Bool m_enableScorches;                                 // +0x20 EnableScorches
	UnsignedByte m_unk0x21[0x3];
	UnsignedByte m_scorchFrequency[0xC];                   // +0x24 ScorchFrequency; unresolved: INI::parseGameClientRandomVariable
	UnsignedByte m_scorchSize[0xC];                        // +0x30 ScorchSize; unresolved: INI::parseGameClientRandomVariable
	Real m_scorchIntensity;                                // +0x3C ScorchIntensity
};
typedef char Fire_off_m_terrainFireSystem[(offsetof(Fire, m_terrainFireSystem) == 0x0) ? 1 : -1];
typedef char Fire_off_m_terrainSmokeSystem[(offsetof(Fire, m_terrainSmokeSystem) == 0x4) ? 1 : -1];
typedef char Fire_off_m_burntTerrainColor[(offsetof(Fire, m_burntTerrainColor) == 0x8) ? 1 : -1];
typedef char Fire_off_m_fuelIndicatorColor[(offsetof(Fire, m_fuelIndicatorColor) == 0x14) ? 1 : -1];
typedef char Fire_off_m_enableScorches[(offsetof(Fire, m_enableScorches) == 0x20) ? 1 : -1];
typedef char Fire_off_m_scorchFrequency[(offsetof(Fire, m_scorchFrequency) == 0x24) ? 1 : -1];
typedef char Fire_off_m_scorchSize[(offsetof(Fire, m_scorchSize) == 0x30) ? 1 : -1];
typedef char Fire_off_m_scorchIntensity[(offsetof(Fire, m_scorchIntensity) == 0x3C) ? 1 : -1];

#endif
