#pragma once
#include "GameLogic/PartitionFilter.h"
class Rva002614DFFilter : public PartitionFilter
{
public:
 Rva002614DFFilter(const Object *obj) : m_obj(obj) {}
 virtual bool allow(Object *);
 const Object *m_obj; // 0x08
};
