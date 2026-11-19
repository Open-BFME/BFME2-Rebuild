#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterOnMap: accepts the objects that are not off the map (allow 0x002611DD tests bit 3 of
// Object +0x438, the bit PartitionFilterSameMapStatus compares).
class PartitionFilterOnMap : public PartitionFilter
{
public:
 PartitionFilterOnMap() {}
 virtual bool allow(Object *);	// 0x002611DD
};
// SIZE 0x08
