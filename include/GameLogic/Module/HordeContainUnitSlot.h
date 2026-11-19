// HordeContainUnitSlot: one banner-carrier position of a horde (HordeContain's
// BannerCarrierPositions entries: "UnitType:<name> Pos:X:<x> Y:<y>"). SIZE 0xC.
// Target facts:
//  - ctor 0x0046AA2F (COMDAT): builds the string at +0 from "" (StringBase<char>(const char *)
//    0x00037BA0, the "" at RVA 0x007BAC1C) and zeroes the two floats at +4 and +8.
//  - parseBannerCarrierPosition 0x0046F179 news 0xC bytes, sets +0 from the UnitType token
//    (StringBase<char>::set(const char *) 0x000055F5) and copies the parsed Coord2D to +4/+8,
//    then pushes the pointer onto the module data's vector (+0x20C).
#pragma once
#ifndef BFME2_HORDECONTAINUNITSLOT_H
#define BFME2_HORDECONTAINUNITSLOT_H

#include "Common/AsciiString.h"
#include "Common/Coord2D.h"

class HordeContainUnitSlot
{
public:
	HordeContainUnitSlot() : m_unitType("") { m_pos.x = 0.0f; m_pos.y = 0.0f; }

	AsciiString m_unitType;	// +0x00 the carrier's thing template name
	Coord2D m_pos;			// +0x04 its offset in the formation
};
// SIZE 0xC
typedef char HordeContainUnitSlotSize[(sizeof(HordeContainUnitSlot) == 0xC) ? 1 : -1];

// HordeContainSplitEntry: one SplitHordes line ("SplitResult:<name> UnitType:<name>
// [RankNumber:<n>]"). SIZE 0xC. Role name: the map has no row of its own for it.
// Target facts: parseHordeContainSplitResult 0x0046F288 news 0xC bytes, clears the two
// string pointers at +0/+4 in place (the implicit constructor, no EH frame), sets them
// from the two tokens and +8 from atoi of the optional RankNumber (0 without it), then
// pushes the pointer onto the module data's vector (+0x1A4).
class HordeContainSplitEntry
{
public:
	AsciiString m_splitResult;	// +0x00
	AsciiString m_unitType;		// +0x04
	Int m_rankNumber;			// +0x08
};
typedef char HordeContainSplitEntrySize[(sizeof(HordeContainSplitEntry) == 0xC) ? 1 : -1];

#endif
