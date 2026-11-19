// DRAFT (h05 fpgen): AutoResolveLeadership from retail's field-parse table (VA 0x00C3A730+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x15; bases and vptr, if any, are the leading gap.
// Unresolved members: m_affects (+0x4, Rva00417DFB::parseFromINI), m_bonusForLevel (+0x8, Rva00417FF0Parse).
#pragma once
#ifndef BFME2_AUTO_RESOLVE_LEADERSHIP_H
#define BFME2_AUTO_RESOLVE_LEADERSHIP_H

#include <stddef.h>
#include "Common/GameType.h"


class AutoResolveLeadership
{
public:

	UnsignedByte m_unk0x0[0x4];
	UnsignedByte m_affects[0x4];                           // +0x4 Affects; unresolved: Rva00417DFB::parseFromINI
	UnsignedByte m_bonusForLevel[0xC];                     // +0x8 BonusForLevel; unresolved: Rva00417FF0Parse
	Bool m_affectsHigherLevelFirst;                        // +0x14 AffectsHigherLevelFirst
};
typedef char AutoResolveLeadership_off_m_affects[(offsetof(AutoResolveLeadership, m_affects) == 0x4) ? 1 : -1];
typedef char AutoResolveLeadership_off_m_bonusForLevel[(offsetof(AutoResolveLeadership, m_bonusForLevel) == 0x8) ? 1 : -1];
typedef char AutoResolveLeadership_off_m_affectsHigherLevelFirst[(offsetof(AutoResolveLeadership, m_affectsHigherLevelFirst) == 0x14) ? 1 : -1];

#endif
