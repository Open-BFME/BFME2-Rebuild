// PartitionFilterFlammable.h: the partition filter that accepts objects whose FlammableUpdate would ignite.
// Target facts: allow 0x0048B6D5 is the public virtual override of PartitionFilter::allow (slot 1 after the
// dtor); the class adds no data, so its size is the base's (vptr and m_next).
#pragma once
#ifndef BFME2_PARTITION_FILTER_FLAMMABLE_H
#define BFME2_PARTITION_FILTER_FLAMMABLE_H

#include "GameLogic/PartitionFilter.h"

class Object;

class PartitionFilterFlammable : public PartitionFilter
{
public:
	virtual bool allow(Object *obj);	// 0x0048B6D5
};

#endif
