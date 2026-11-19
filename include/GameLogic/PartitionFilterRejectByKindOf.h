#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/BitFlags.h"
// Zero Hour's PartitionFilterRejectByKindOf (allow 0x0026115D: the candidate's kind-of mask against the two
// masks through Thing::isKindOfMulti 0x0030AD7D, result negated). The masks are BitFlags<218>, 0x1C bytes each.
class PartitionFilterRejectByKindOf : public PartitionFilter
{
public:
 PartitionFilterRejectByKindOf(const KindOfMaskType &,const KindOfMaskType &) throw();
 virtual bool allow(Object *);
 KindOfMaskType m_required; // 0x08
 KindOfMaskType m_forbidden; // 0x24
};
// SIZE 0x40
