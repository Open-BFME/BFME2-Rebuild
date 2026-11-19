#pragma once
#include "GameLogic/PartitionFilter.h"
class UpgradeTemplate;
// A filter on one upgrade: accepts the objects the upgrade applies to that do not have it yet. Retail's class name is
// unknown (Zero Hour has no such filter); the role name is the behaviour. Target facts: allow 0x00261130 is
// `other->affectedByUpgrade(m_upgrade) && !other->hasUpgrade(m_upgrade)` with the template at +8.
class PartitionFilterUpgradeable : public PartitionFilter
{
public:
 PartitionFilterUpgradeable(const UpgradeTemplate *upgrade) : m_upgrade(upgrade) {}
 virtual bool allow(Object *);	// 0x00261130
 const UpgradeTemplate *m_upgrade;	// 0x08
};
// SIZE 0x0C
