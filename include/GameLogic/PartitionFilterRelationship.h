#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterRelationship: accepts the objects whose relationship to m_obj is one of the chosen ones
// (m_flags holds 1 << Relationship bits). Target facts: vtable 0x007FBC90 (allow 0x00260EB1, getPlayerMask
// 0x00260E6A). BFME 2 adds the byte at +0x10: set, allow asks the candidate for its relationship to m_obj instead of
// the other way round, and getPlayerMask hands it to PlayerList::getPlayersWithRelationship.
class PartitionFilterRelationship : public PartitionFilter
{
public:
 enum RelationshipAllowTypes	// Zero Hour's
 {
  ALLOW_ALLIES = (1<<2),	// 1 << ALLIES
  ALLOW_ENEMIES = (1<<0),	// 1 << ENEMIES
  ALLOW_NEUTRAL = (1<<1)	// 1 << NEUTRAL
 };
 PartitionFilterRelationship(const Object *obj, int flags, bool match) : m_obj(obj),m_flags(flags),m_match(match) {}
 virtual bool allow(Object *);
 virtual int getPlayerMask();
 const Object *m_obj; // 0x08
 int m_flags; // 0x0C
 bool m_match; // 0x10
};
// SIZE 0x14
