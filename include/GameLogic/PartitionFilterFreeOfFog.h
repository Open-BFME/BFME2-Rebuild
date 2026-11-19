// Zero Hour's PartitionFilterFreeOfFog (h11's header; its allow row 0x00261353 now proves the shape).
// Target facts: vtable RVA 0x0080716C, allow 0x00261353 (21 bytes): the candidate's
// Object::getShroudStatusForPlayer(m_playerIndex) (0x0028D2A2) equals 1 (clear). Member +8 is the
// player index (AI::rva002FEEAD 0x002FEEAD builds it from the controlling player's index).
#pragma once
#include "GameLogic/PartitionFilter.h"
class PartitionFilterFreeOfFog : public PartitionFilter
{
public:
 PartitionFilterFreeOfFog(int playerIndex) : m_playerIndex(playerIndex) {}
 virtual bool allow(Object *);
 int m_playerIndex;	// 0x08
};
// SIZE 0x0C
