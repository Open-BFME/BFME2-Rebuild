// Draft/Rva00260E2AFilter.h: the partition filter retail's RespawnUpdate::searchForSuitableSpawnAtObject
// 0x004AF1CE builds inline on its stack: VA 0x007FAD04 is stored at +0, +4 is zeroed (the chain link) and
// the controlling player of the searching object goes to +8.
// Target facts: the class's own table has three slots, the scalar-deleting destructor 0x00395A19,
// allow 0x00260E2A (ledger name Rva00260E2AFilter::allow) and getPlayerMask 0x00260E1E (ledger name
// Rva00260E2AFilter::getPlayerMask). Role of the player is not established: allow's body is not read.
#pragma once
#include "GameLogic/PartitionFilter.h"
class Player;
class Rva00260E2AFilter : public PartitionFilter
{
public:
  Rva00260E2AFilter(Player *player) : m_player(player) {}
  virtual bool allow(Object *object); // 0x00260E2A
  virtual int getPlayerMask(); // 0x00260E1E
  Player *m_player; // +0x08
};
// SIZE 0x0C
