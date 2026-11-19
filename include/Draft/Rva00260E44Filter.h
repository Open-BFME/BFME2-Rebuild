// Draft/Rva00260E44Filter.h: the partition filter that SalvageCrateCollide::executeCrateBehavior 0x004BD442 chains
// behind Rva00260E2AFilter (retail's table 0x00C5A99C: the scalar-deleting destructor 0x00395A19 and allow 0x00260E44;
// no members beyond the PartitionFilter link). Proven by that row: the object it builds is chained with
// PartitionFilter::And (0x00625790), so it is a PartitionFilter.
#pragma once
#include "GameLogic/PartitionFilter.h"
class Rva00260E44Filter : public PartitionFilter
{
public:
  virtual bool allow(Object *object); // 0x00260E44
};
// SIZE 0x08
