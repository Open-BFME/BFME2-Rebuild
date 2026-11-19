// DRAFT (h05 fpgen): AIBase from retail's field-parse table (VA 0x00C3AF18+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x19; bases and vptr, if any, are the leading gap.
// Unresolved members: m_playerPositions (+0xC, INI::Rva004E8EFF_ParseDisabledSlots).
#pragma once
#ifndef BFME2_A_I_BASE_H
#define BFME2_A_I_BASE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class AIBase
{
public:

	AsciiString m_side;                                    // +0x0 Side
	AsciiString m_map;                                     // +0x4 Map
	AsciiString m_gameMapToUseOn;                          // +0x8 GameMapToUseOn
	UnsignedByte m_playerPositions[0xC];                   // +0xC PlayerPositions; unresolved: INI::Rva004E8EFF_ParseDisabledSlots
	Bool m_allowsArbirtaryRotation;                        // +0x18 AllowsArbirtaryRotation
};
typedef char AIBase_off_m_side[(offsetof(AIBase, m_side) == 0x0) ? 1 : -1];
typedef char AIBase_off_m_map[(offsetof(AIBase, m_map) == 0x4) ? 1 : -1];
typedef char AIBase_off_m_gameMapToUseOn[(offsetof(AIBase, m_gameMapToUseOn) == 0x8) ? 1 : -1];
typedef char AIBase_off_m_playerPositions[(offsetof(AIBase, m_playerPositions) == 0xC) ? 1 : -1];
typedef char AIBase_off_m_allowsArbirtaryRotation[(offsetof(AIBase, m_allowsArbirtaryRotation) == 0x18) ? 1 : -1];

#endif
