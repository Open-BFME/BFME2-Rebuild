#pragma once
#include "GameLogic/Module/AutoPickUpEatObjectEntryVectorBase.h"
#include "Common/FalseType.h"
class AutoPickUpEatObjectEntryVector : public AutoPickUpEatObjectEntryVectorBase
{
public:
 AutoPickUpEatObjectEntryVector() : AutoPickUpEatObjectEntryVectorBase(AutoPickUpEntryAllocator()) {}
 ~AutoPickUpEatObjectEntryVector();
 unsigned size() const { return m_finish-m_start; }
 void clearStorage();
 void push_back(const AutoPickUpEatObjectEntry &);
 void insertOverflow(AutoPickUpEatObjectEntry *,const AutoPickUpEatObjectEntry &,const FalseType &,unsigned,bool);
};
