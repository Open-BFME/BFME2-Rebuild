#pragma once
#include "GameLogic/PartitionFilter.h"
class Player;
// Zero Hour's PartitionFilterPlayer: accepts the objects one player controls (or, with m_match false, everything
// else). Target facts: allow 0x0026137E (MAE, protected) is `(m_player == other->getControllingPlayer()) == m_match`;
// getPlayerMask 0x00261368 (slot 2) is 1 << m_player's index (+0x54), complemented when m_match is false.
class PartitionFilterPlayer : public PartitionFilter
{
public:
 PartitionFilterPlayer(const Player *player, bool match) : m_player(player), m_match(match) {}
 virtual int getPlayerMask();	// 0x00261368
protected:
 virtual bool allow(Object *);	// 0x0026137E
public:
 const Player *m_player;	// 0x08
 bool m_match;	// 0x0C
};
// SIZE 0x10
