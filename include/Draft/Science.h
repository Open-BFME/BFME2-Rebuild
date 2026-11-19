// DRAFT (h05 fpgen): Science from retail's field-parse table (VA 0x00BE2650+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x31; bases and vptr, if any, are the leading gap.
// Unresolved members: m_displayName (+0x14, INI::parseAndTranslateLabel), m_description (+0x18, INI::parseAndTranslateLabel), m_prerequisiteSciences (+0x1C, iniParseScienceVectorVector).
#pragma once
#ifndef BFME2_SCIENCE_H
#define BFME2_SCIENCE_H

#include <stddef.h>
#include "Common/GameType.h"


class Science
{
public:

	UnsignedByte m_unk0x0[0x14];
	UnsignedByte m_displayName[0x4];                       // +0x14 DisplayName; unresolved: INI::parseAndTranslateLabel
	UnsignedByte m_description[0x4];                       // +0x18 Description; unresolved: INI::parseAndTranslateLabel
	UnsignedByte m_prerequisiteSciences[0xC];              // +0x1C PrerequisiteSciences; unresolved: iniParseScienceVectorVector
	Int m_sciencePurchasePointCost;                        // +0x28 SciencePurchasePointCost
	Int m_sciencePurchasePointCostMP;                      // +0x2C SciencePurchasePointCostMP
	Bool m_isGrantable;                                    // +0x30 IsGrantable
};
typedef char Science_off_m_displayName[(offsetof(Science, m_displayName) == 0x14) ? 1 : -1];
typedef char Science_off_m_description[(offsetof(Science, m_description) == 0x18) ? 1 : -1];
typedef char Science_off_m_prerequisiteSciences[(offsetof(Science, m_prerequisiteSciences) == 0x1C) ? 1 : -1];
typedef char Science_off_m_sciencePurchasePointCost[(offsetof(Science, m_sciencePurchasePointCost) == 0x28) ? 1 : -1];
typedef char Science_off_m_sciencePurchasePointCostMP[(offsetof(Science, m_sciencePurchasePointCostMP) == 0x2C) ? 1 : -1];
typedef char Science_off_m_isGrantable[(offsetof(Science, m_isGrantable) == 0x30) ? 1 : -1];

#endif
