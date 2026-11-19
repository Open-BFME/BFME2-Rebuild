#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/ObjectStatusMask.h"
class PartitionFilterStatus : public PartitionFilter
{
public:
 PartitionFilterStatus(const ObjectStatusMask &,const ObjectStatusMask &) throw();
 virtual bool allow(Object *);
 ObjectStatusMask m_required; // 0x08
 ObjectStatusMask m_forbidden; // 0x18
};
