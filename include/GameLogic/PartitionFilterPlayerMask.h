#pragma once
#include "GameLogic/PartitionFilter.h"
// A filter on the candidate's controlling player: accepts the objects whose player is in a player mask (or, with
// m_match false, the ones whose player is not). Retail's class name is unknown (Zero Hour has no such filter); the role
// name is the behaviour. Target facts: vtable 0x0081FDE0, allow 0x002613AF tests bit `player index` of the mask at +8,
// getPlayerMask 0x002613A3 hands the mask back (complemented when m_match is false).
class PartitionFilterPlayerMask : public PartitionFilter
{
public:
 PartitionFilterPlayerMask(int mask, bool match) : m_mask(mask), m_match(match) {}
 virtual bool allow(Object *);	// 0x002613AF
 virtual int getPlayerMask();	// 0x002613A3
 int m_mask;	// 0x08
 bool m_match;	// 0x0C
};
// SIZE 0x10
