#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterRejectBuildings. vtable 0x007F8FF0, allow 0x00261B7E,
// out-of-line constructor 0x002611F2 (stores m_obj and sets m_flag when m_obj's
// controlling player has type 1, +0x5C).
class PartitionFilterRejectBuildings : public PartitionFilter
{
public:
 PartitionFilterRejectBuildings(Object *obj);	// 0x002611F2
 virtual bool allow(Object *);
 Object *m_obj;	// 0x08
 bool m_flag;	// 0x0C
};
// SIZE 0x10
