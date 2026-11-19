// RankInfoStore.h: the table of rank definitions (SubsystemInterface; its vtable has reset at slot 9, 0x002000A4).
// Target facts: the store keeps its RankInfo pointers in a vector at +0xC (begin +0xC, end +0x10; rank levels are 1-based
// indices, see getRankInfo 0x002000D7 and friend_parseRankDefinition 0x0020038E which indexes `[+0xC + level*4 - 4]`).
// reset 0x002000A4 walks the vector erasing every rank that has no override left (Zero Hour's RankInfoStore::reset).
// The constructor, destructor and init (0x001FF8FB, folded with ScienceStore's) are not written yet, so the class size is
// not claimed here.
#pragma once
#ifndef BFME2_RANKINFOSTORE_H
#define BFME2_RANKINFOSTORE_H

#include <vector>
#include "Common/GameType.h"
#include "Common/SubsystemInterface.h"
#include "GameLogic/RankInfo.h"

class RankInfoStore : public SubsystemInterface
{
public:
	virtual void init();									// slot 1, 0x001FF8FB (folded with ScienceStore::init)
	virtual void reset();									// slot 9, 0x002000A4
	virtual void update();									// slot 10, the folded empty body 0x000B3FD0

	RankInfo *getRankInfo(Int rank) const;					// 0x002000D7: the final override of the 1-based rank, or NULL

	_STL::vector<RankInfo *> m_rankInfos;					// +0xC
};

#endif
