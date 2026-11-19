#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/GameType.h"
class Player;
// Zero Hour's PartitionFilterGarrisonableByPlayer: accepts the objects a player may garrison (ActionManager::
// canPlayerGarrison 0x0041C21B), compared with m_match. Target facts: allow 0x00261478 reads the player at +8, the
// match byte at +0xC and the command source at +0x10.
class PartitionFilterGarrisonableByPlayer : public PartitionFilter
{
public:
 PartitionFilterGarrisonableByPlayer(const Player *player, bool match, UnsignedInt commandSource)
  : m_player(player), m_match(match), m_commandSource(commandSource) {}
 virtual bool allow(Object *);	// 0x00261478
 const Player *m_player;	// 0x08
 bool m_match;	// 0x0C
 UnsignedInt m_commandSource;	// 0x10
};
// SIZE 0x14
