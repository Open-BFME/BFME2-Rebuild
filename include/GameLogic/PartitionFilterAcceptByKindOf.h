#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/BitFlags.h"
class PartitionFilterAcceptByKindOf : public PartitionFilter
{
public:
 PartitionFilterAcceptByKindOf(const KindOfMaskType &,const KindOfMaskType &) throw();
 virtual bool allow(Object *);
 KindOfMaskType m_required; // 0x08
 KindOfMaskType m_forbidden; // 0x24
};
