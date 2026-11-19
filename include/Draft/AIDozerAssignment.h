// DRAFT (h05 fpgen): AIDozerAssignment from retail's field-parse table (VA 0x00BFD7C0+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x8; bases and vptr, if any, are the leading gap.
// Unresolved members: none.
#pragma once
#ifndef BFME2_A_I_DOZER_ASSIGNMENT_H
#define BFME2_A_I_DOZER_ASSIGNMENT_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class AIDozerAssignment
{
public:

	AsciiString m_side;                                    // +0x0 Side
	AsciiString m_unit;                                    // +0x4 Unit
};
typedef char AIDozerAssignment_off_m_side[(offsetof(AIDozerAssignment, m_side) == 0x0) ? 1 : -1];
typedef char AIDozerAssignment_off_m_unit[(offsetof(AIDozerAssignment, m_unit) == 0x4) ? 1 : -1];

#endif
