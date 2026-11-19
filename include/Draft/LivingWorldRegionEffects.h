// DRAFT (h05 fpgen): LivingWorldRegionEffects from retail's field-parse table (VA 0x00DC14C8+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x55; bases and vptr, if any, are the leading gap.
// Unresolved members: m_regionObject (+0x1C, INI::Rva003EEABF_ParseAsciiString), m_neutralRegionColor (+0x20, INI::parseRGBColor), m_regionBorderColor (+0x2C, INI::parseRGBColor), m_shellStartPositionColor (+0x38, INI::parseRGBColor).
#pragma once
#ifndef BFME2_LIVING_WORLD_REGION_EFFECTS_H
#define BFME2_LIVING_WORLD_REGION_EFFECTS_H

#include <stddef.h>
#include "Common/GameType.h"


class LivingWorldRegionEffects
{
public:

	UnsignedByte m_unk0x0[0x1C];
	UnsignedByte m_regionObject[0x4];                      // +0x1C RegionObject; unresolved: INI::Rva003EEABF_ParseAsciiString
	UnsignedByte m_neutralRegionColor[0xC];                // +0x20 NeutralRegionColor; unresolved: INI::parseRGBColor
	UnsignedByte m_regionBorderColor[0xC];                 // +0x2C RegionBorderColor; unresolved: INI::parseRGBColor
	UnsignedByte m_shellStartPositionColor[0x1C];          // +0x38 ShellStartPositionColor; unresolved: INI::parseRGBColor
	Bool m_shouldShowRegionBorders;                        // +0x54 ShouldShowRegionBorders
};
typedef char LivingWorldRegionEffects_off_m_regionObject[(offsetof(LivingWorldRegionEffects, m_regionObject) == 0x1C) ? 1 : -1];
typedef char LivingWorldRegionEffects_off_m_neutralRegionColor[(offsetof(LivingWorldRegionEffects, m_neutralRegionColor) == 0x20) ? 1 : -1];
typedef char LivingWorldRegionEffects_off_m_regionBorderColor[(offsetof(LivingWorldRegionEffects, m_regionBorderColor) == 0x2C) ? 1 : -1];
typedef char LivingWorldRegionEffects_off_m_shellStartPositionColor[(offsetof(LivingWorldRegionEffects, m_shellStartPositionColor) == 0x38) ? 1 : -1];
typedef char LivingWorldRegionEffects_off_m_shouldShowRegionBorders[(offsetof(LivingWorldRegionEffects, m_shouldShowRegionBorders) == 0x54) ? 1 : -1];

#endif
