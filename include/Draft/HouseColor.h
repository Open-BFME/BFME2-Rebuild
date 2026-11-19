// DRAFT (h05 fpgen): HouseColor from retail's field-parse table (VA 0x00C3A600+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x14; bases and vptr, if any, are the leading gap.
// Unresolved members: none.
#pragma once
#ifndef BFME2_HOUSE_COLOR_H
#define BFME2_HOUSE_COLOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class HouseColor
{
public:

	UnsignedByte m_unk0x0[0xC];
	AsciiString m_houseTexture;                            // +0xC HouseTexture
	AsciiString m_baseTexture;                             // +0x10 BaseTexture
};
typedef char HouseColor_off_m_houseTexture[(offsetof(HouseColor, m_houseTexture) == 0xC) ? 1 : -1];
typedef char HouseColor_off_m_baseTexture[(offsetof(HouseColor, m_baseTexture) == 0x10) ? 1 : -1];

#endif
