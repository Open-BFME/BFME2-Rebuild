#pragma once
#include "GameLogic/Module/RiderInfo.h"
class RiderInfoAllocator
{
public:
 RiderInfo *allocate(unsigned int count,const void *hint=0) const;
};
