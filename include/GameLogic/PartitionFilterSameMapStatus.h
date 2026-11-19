#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterSameMapStatus (the map keeps the name on allow 0x002611BF).
// vtable 0x007F91BC; allow compares the map-status bit (+0x438 bit 3) of the
// candidate with that of m_obj's. Replaces PartitionFilterSameMap.
class PartitionFilterSameMapStatus : public PartitionFilter
{
public:
 PartitionFilterSameMapStatus(const Object *obj) : m_obj(obj) {}
 virtual bool allow(Object *);
 const Object *m_obj; // 0x08
};
// SIZE 0x0C
