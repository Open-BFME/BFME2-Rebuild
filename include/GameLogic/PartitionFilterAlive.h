#pragma once
#include "GameLogic/PartitionFilter.h"
class PartitionFilterAlive : public PartitionFilter
{
public:
 virtual bool allow(Object *);
};
