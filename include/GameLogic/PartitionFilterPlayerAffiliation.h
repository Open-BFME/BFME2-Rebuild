#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/GameType.h"
class Player;

// Zero Hour's AllowPlayerRelationship (its PlayerList.h): the relationships the filter selects by. Retail's allow
// 0x00261409 tests 2, 4 and 8, and getPlayerMask 0x002613E1 always adds the player itself (1). It lives here until
// PlayerList.h's owner takes it.
enum AllowPlayerRelationship
{
	ALLOW_SAME_PLAYER = 0x01,	///< allow only objects of the same player as the filter's player
	ALLOW_ALLIES = 0x02,		///< allow objects that the player considers allies
	ALLOW_ENEMIES = 0x04,		///< allow objects that the player considers enemies
	ALLOW_NEUTRAL = 0x08		///< allow objects that the player considers neutral
};

// Zero Hour's PartitionFilterPlayerAffiliation: accepts (or, with m_match false, rejects) the objects whose team has one
// of the chosen relationships to a player (AllowPlayerRelationship flags). Target facts: vtable 0x008004D8 (allow
// 0x00261409, getPlayerMask 0x002613E1); the inline constructors of the command-button action rows store the
// player at +8, the match byte at +0xC and the flags at +0x10 (4 = ALLOW_ENEMIES).
class PartitionFilterPlayerAffiliation : public PartitionFilter
{
public:
 PartitionFilterPlayerAffiliation(const Player *player, UnsignedInt whichAffiliation, bool match)
  : m_player(player), m_match(match), m_affiliation(whichAffiliation) {}
 virtual bool allow(Object *);	// 0x00261409
 virtual int getPlayerMask();	// 0x002613E1
 const Player *m_player;	// 0x08
 bool m_match;	// 0x0C
 UnsignedInt m_affiliation;	// 0x10
};
// SIZE 0x14
