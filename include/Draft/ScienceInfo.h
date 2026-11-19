// DRAFT: the ScienceInfo record a ScienceStore lookup returns. Proven so far:
// isScienceGrantable (RVA 0x001FF432) reads one byte at +0x30 of the record it finds.
// Other members are not established by any row yet.
#pragma once
#include "Common/GameType.h"
#include "Common/Overridable.h"
#include "Common/Science.h"
#include "Common/UnicodeString.h"
#include <vector>

// An Overridable (s02: ScienceStore::findScienceInfo 0x001FF3AD reads each entry through the
// inline +4 next-override test and friend_getFinalOverride 0x001E35DF, then compares +0x10
// with the science it looks for).
class ScienceInfo : public Overridable
{
public:
	ScienceType m_science;			// +0x10 the science this record describes (findScienceInfo's key)
	UnicodeString m_displayName;	// +0x14 copied by getNameAndDescription (0x001FF449)
	UnicodeString m_description;	// +0x18 copied by getNameAndDescription (0x001FF449)
	_STL::vector<_STL::vector<ScienceType> > m_prerequisites;	// +0x1C alternatives: playerHasPrereqsForScience 0x001FF47D is satisfied when every science of one inner list is known
	Int m_unk0x28;					// +0x28 purchase cost read by getSciencePurchaseCost 0x001FF3DC (single player)
	Int m_unk0x2C;					// +0x2C purchase cost read by getSciencePurchaseCost (multiplayer)
	Bool m_unk0x30;				// +0x30 read as the grantable flag by isScienceGrantable (0x001FF432)
};
