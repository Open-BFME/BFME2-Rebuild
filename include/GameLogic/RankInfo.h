// RankInfo.h: one rank definition of the rank table (an Overridable, 0x44 bytes).
// Target facts: ctor 0x0020010B stores Overridable's fields and the table 0x00BE26E0, nulls the UnicodeString at +0x10 (its unwind funclets call the wide releaseBuffer fold 0x005B804E), constructs the
// vector at +0x38, sets seven words at +0x18..+0x30 to -1 and +0x14 / +0x34 to 0. The destructor 0x00200236 frees the vector storage
// (direct `call _free`), releases the string and runs ~Overridable. The member names are role-less offsets: friend_parseRankDefinition
// 0x0020038E (not written) owns the INI fields.
#pragma once
#ifndef BFME2_RANKINFO_H
#define BFME2_RANKINFO_H

#include <vector>
#include "Common/GameType.h"
#include "Common/UnicodeString.h"
#include "Common/Overridable.h"

class RankInfo : public Overridable
{
public:
	RankInfo();													// 0x0020010B
	// ~RankInfo 0x00200236 is the implicit destructor: retail's body does not reset the vtable pointer.

	UnicodeString m_unk0x10;										// +0x10
	Int m_unk0x14;												// +0x14 (0)
	Int m_unk0x18;												// +0x18 .. +0x30: seven words, all -1 after construction
	Int m_unk0x1C;
	Int m_unk0x20;
	Int m_unk0x24;
	Int m_unk0x28;
	Int m_unk0x2C;
	Int m_unk0x30;
	Int m_sciencePurchasePointsGranted;							// +0x34 (0): Player::grantRankBonuses 0x002AD7E9 adds it to the score keeper; SciencePurchasePointTracker::resetRank 0x0038028B to the points
	std::vector<Int> m_sciencesGranted;							// +0x38 the ScienceType list: Player::grantRankBonuses 0x002AD7E9 and Player::rva002AE252 hand each to addScience
};
// SIZE 0x44
typedef char RankInfo_size[(sizeof(RankInfo) == 0x44) ? 1 : -1];

#endif
