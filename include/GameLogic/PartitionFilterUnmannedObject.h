#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterUnmannedObject: accepts the objects that are disabled as unmanned (or, with m_match false,
// the ones that are not). Target facts: vtable 0x0081FE28, allow 0x00261C58 reads bit 5 (DISABLED_UNMANNED) of the
// disabled mask at Object +0x1C8 and compares it with the byte at +8.
class PartitionFilterUnmannedObject : public PartitionFilter
{
public:
 PartitionFilterUnmannedObject(bool match) : m_match(match) {}
 virtual bool allow(Object *);	// 0x00261C58
 bool m_match;	// 0x08
};
// SIZE 0x0C
