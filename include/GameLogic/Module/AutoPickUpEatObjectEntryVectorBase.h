#pragma once
#include "GameLogic/Module/AutoPickUpEatObjectEntry.h"
#include <stdlib.h>
struct AutoPickUpEntryAllocator { AutoPickUpEntryAllocator() {} AutoPickUpEatObjectEntry *allocate(unsigned,const void * = 0) const; };
class AutoPickUpEatObjectEntryVectorBase
{
public:
 AutoPickUpEatObjectEntryVectorBase(const AutoPickUpEntryAllocator &) throw();
 ~AutoPickUpEatObjectEntryVectorBase() { if(m_start) free(m_start); }
 AutoPickUpEatObjectEntry *m_start;
 AutoPickUpEatObjectEntry *m_finish;
 AutoPickUpEatObjectEntry *m_end;
};
// SIZE 0x0C
