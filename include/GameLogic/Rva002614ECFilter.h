#pragma once
#include "GameLogic/PartitionFilter.h"
class Player;
class Rva002614ECFilter : public PartitionFilter
{
public:
 Rva002614ECFilter(const void *what,Player *player,bool match) : m_what(what),m_player(player),m_match(match) {}
 virtual bool allow(Object *);

 const void *m_what;
 Player *m_player;
 bool m_match;
};
