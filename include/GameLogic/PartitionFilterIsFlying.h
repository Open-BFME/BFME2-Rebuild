#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterIsFlying: accepts objects that use an airborne locomotor. allow 0x00260E01 is
// `return other->isUsingAirborneLocomotor()` (Object 0x0028B81E).
class PartitionFilterIsFlying : public PartitionFilter
{
public:
 PartitionFilterIsFlying() {}
 virtual bool allow(Object *);	// 0x00260E01
};
// SIZE 0x08
