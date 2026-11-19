// DRAFT (h05 fpgen): AutoResolveBody from retail's field-parse table (VA 0x00C3A8B0+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x1E; bases and vptr, if any, are the leading gap.
// Unresolved members: m_hitpointsAtLevel (+0x0, Rva004185E4HitpointsAtLevel), m_regenerateAtLevel (+0xC, Rva0041865FRegenerateAtLevel).
#pragma once
#ifndef BFME2_AUTO_RESOLVE_BODY_H
#define BFME2_AUTO_RESOLVE_BODY_H

#include <stddef.h>
#include "Common/GameType.h"


class AutoResolveBody
{
public:

	UnsignedByte m_hitpointsAtLevel[0xC];                  // +0x0 HitpointsAtLevel; unresolved: Rva004185E4HitpointsAtLevel
	UnsignedByte m_regenerateAtLevel[0x10];                // +0xC RegenerateAtLevel; unresolved: Rva0041865FRegenerateAtLevel
	Bool m_canBeAttacked;                                  // +0x1C CanBeAttacked
	Bool m_leaveInArmySummary;                             // +0x1D LeaveInArmySummary
};
typedef char AutoResolveBody_off_m_hitpointsAtLevel[(offsetof(AutoResolveBody, m_hitpointsAtLevel) == 0x0) ? 1 : -1];
typedef char AutoResolveBody_off_m_regenerateAtLevel[(offsetof(AutoResolveBody, m_regenerateAtLevel) == 0xC) ? 1 : -1];
typedef char AutoResolveBody_off_m_canBeAttacked[(offsetof(AutoResolveBody, m_canBeAttacked) == 0x1C) ? 1 : -1];
typedef char AutoResolveBody_off_m_leaveInArmySummary[(offsetof(AutoResolveBody, m_leaveInArmySummary) == 0x1D) ? 1 : -1];

#endif
