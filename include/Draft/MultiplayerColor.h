// DRAFT (h05 fpgen): MultiplayerColor from retail's field-parse table (VA 0x00C18F40+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x3D; bases and vptr, if any, are the leading gap.
// Unresolved members: m_rgbColor (+0x4, INI::parseRGBColor), m_rgbNightColor (+0x14, INI::parseRGBColor), m_livingWorldColor (+0x24, INI::parseRGBColor), m_livingWorldBannerColor (+0x30, INI::parseRGBColor).
#pragma once
#ifndef BFME2_MULTIPLAYER_COLOR_H
#define BFME2_MULTIPLAYER_COLOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class MultiplayerColor
{
public:

	AsciiString m_tooltipName;                             // +0x0 TooltipName
	UnsignedByte m_rgbColor[0x10];                         // +0x4 RGBColor; unresolved: INI::parseRGBColor
	UnsignedByte m_rgbNightColor[0x10];                    // +0x14 RGBNightColor; unresolved: INI::parseRGBColor
	UnsignedByte m_livingWorldColor[0xC];                  // +0x24 LivingWorldColor; unresolved: INI::parseRGBColor
	UnsignedByte m_livingWorldBannerColor[0xC];            // +0x30 LivingWorldBannerColor; unresolved: INI::parseRGBColor
	Bool m_availableInWotR;                                // +0x3C AvailableInWotR
};
typedef char MultiplayerColor_off_m_tooltipName[(offsetof(MultiplayerColor, m_tooltipName) == 0x0) ? 1 : -1];
typedef char MultiplayerColor_off_m_rgbColor[(offsetof(MultiplayerColor, m_rgbColor) == 0x4) ? 1 : -1];
typedef char MultiplayerColor_off_m_rgbNightColor[(offsetof(MultiplayerColor, m_rgbNightColor) == 0x14) ? 1 : -1];
typedef char MultiplayerColor_off_m_livingWorldColor[(offsetof(MultiplayerColor, m_livingWorldColor) == 0x24) ? 1 : -1];
typedef char MultiplayerColor_off_m_livingWorldBannerColor[(offsetof(MultiplayerColor, m_livingWorldBannerColor) == 0x30) ? 1 : -1];
typedef char MultiplayerColor_off_m_availableInWotR[(offsetof(MultiplayerColor, m_availableInWotR) == 0x3C) ? 1 : -1];

#endif
