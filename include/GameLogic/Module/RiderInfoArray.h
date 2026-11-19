#pragma once
#include "Common/FalseType.h"
#include "GameLogic/Module/RiderInfo.h"
class RiderInfoArray
{
public:
 RiderInfo *get(int index) const;
 void push_back(const RiderInfo &);
 void insertOverflow(RiderInfo *,const RiderInfo &,const FalseType &,unsigned int,bool);
 RiderInfo *m_begin; // 0x00
 RiderInfo *m_end; // 0x04
 RiderInfo *m_capacity; // 0x08
};
// SIZE 0x0C
