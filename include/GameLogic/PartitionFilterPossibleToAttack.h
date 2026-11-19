#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterPossibleToAttack. vtable 0x007F91B0, allow 0x00260FD0:
// m_obj->getAbleToAttackSpecificObject(m_attackType (+0x10), candidate,
// m_commandSource (+0x0C)), the call's last argument pushed first; accepted for
// results 2 and 3. TunnelNetworkScan's constructor stores 2 at +0x0C (Zero Hour's
// CMD_FROM_AI) and 0 at +0x10, so the constructor takes the command source first.
class PartitionFilterPossibleToAttack : public PartitionFilter
{
public:
 PartitionFilterPossibleToAttack(const Object *obj, int commandSource, int attackType)
  : m_obj(obj), m_commandSource(commandSource), m_attackType(attackType) {}
 virtual bool allow(Object *);
 const Object *m_obj;	// 0x08
 int m_commandSource;	// 0x0C
 int m_attackType;		// 0x10
};
// SIZE 0x14
