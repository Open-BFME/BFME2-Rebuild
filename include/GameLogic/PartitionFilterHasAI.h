#pragma once
#include "GameLogic/PartitionFilter.h"
class PartitionFilterHasAI : public PartitionFilter
{
public:
 virtual bool allow(Object *);
};
