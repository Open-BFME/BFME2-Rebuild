#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/BitFlags.h"
class Rva003959FA : public PartitionFilter
{
public:
 Rva003959FA(const KindOfMaskType &) throw();
 virtual bool allow(Object *);
 KindOfMaskType m_mask;
};
