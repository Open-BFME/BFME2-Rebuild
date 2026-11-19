#pragma once
#include "GameLogic/PartitionFilter.h"
// The opposite of PartitionFilterAlive: accepts the objects that are effectively dead. Retail's class name is unknown
// (Zero Hour has no such filter); the role name is the behaviour. Target facts: allow 0x002611AF returns bit 0 of the
// candidate's private status byte (Object +0x438), which Object::isEffectivelyDead reads.
class PartitionFilterEffectivelyDead : public PartitionFilter
{
public:
 PartitionFilterEffectivelyDead() {}
 virtual bool allow(Object *);	// 0x002611AF
};
// SIZE 0x08
