// DRAFT (h05 fpgen): LivingWorldSound from retail's field-parse table (VA 0x00C37938+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x2C; bases and vptr, if any, are the leading gap.
// Unresolved members: m_sound (+0x14, INI::parseAudioEventRTS), m_flags (+0x18, Rva003FB508::parseFromINI).
#pragma once
#ifndef BFME2_LIVING_WORLD_SOUND_H
#define BFME2_LIVING_WORLD_SOUND_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/Coord2D.h"


class LivingWorldSound
{
public:

	UnsignedByte m_unk0x0[0x8];
	Coord3D m_position;                                    // +0x8 Position
	UnsignedByte m_sound[0x4];                             // +0x14 Sound; unresolved: INI::parseAudioEventRTS
	UnsignedByte m_flags[0x4];                             // +0x18 Flags; unresolved: Rva003FB508::parseFromINI
	Coord2D m_zoomRegionLow;                               // +0x1C ZoomRegionLow
	Coord2D m_zoomRegionHigh;                              // +0x24 ZoomRegionHigh
};
typedef char LivingWorldSound_off_m_position[(offsetof(LivingWorldSound, m_position) == 0x8) ? 1 : -1];
typedef char LivingWorldSound_off_m_sound[(offsetof(LivingWorldSound, m_sound) == 0x14) ? 1 : -1];
typedef char LivingWorldSound_off_m_flags[(offsetof(LivingWorldSound, m_flags) == 0x18) ? 1 : -1];
typedef char LivingWorldSound_off_m_zoomRegionLow[(offsetof(LivingWorldSound, m_zoomRegionLow) == 0x1C) ? 1 : -1];
typedef char LivingWorldSound_off_m_zoomRegionHigh[(offsetof(LivingWorldSound, m_zoomRegionHigh) == 0x24) ? 1 : -1];

#endif
