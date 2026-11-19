#pragma once
#include "GameLogic/PartitionFilter.h"
class Player;
class Rva00261058 : public PartitionFilter
{
public:
 Rva00261058(Object *obj,bool flag);	// 0x00261058 (out of line)
 Rva00261058(Player *player,bool flag) : m_player(player),m_flag(flag) {}
 virtual bool allow(Object *);

 Player *m_player;
 bool m_flag;
};
