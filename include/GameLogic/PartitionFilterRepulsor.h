#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterRepulsor. vtable 0x00807178, allow 0x00261BFB (not
// m_obj itself, not a status-8 object, ..., able to attack); m_obj at +0x08.
class PartitionFilterRepulsor : public PartitionFilter
{
public:
 PartitionFilterRepulsor(const Object *obj) : m_obj(obj) {}
 virtual bool allow(Object *);
 const Object *m_obj;	// 0x08
};
// SIZE 0x0C
