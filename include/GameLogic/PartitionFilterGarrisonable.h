#pragma once
#include "GameLogic/PartitionFilter.h"
// Zero Hour's PartitionFilterGarrisonable: accepts the objects whose contain module can be garrisoned (or, with
// m_match false, the ones that cannot). Target facts: vtable 0x0081FE04, allow 0x0026144C: no contain module
// (Object +0x250) answers `!m_match`, otherwise the module's slot 4 (isGarrisonable) is compared with the byte at +8.
class PartitionFilterGarrisonable : public PartitionFilter
{
public:
 PartitionFilterGarrisonable(bool match) : m_match(match) {}
 virtual bool allow(Object *);	// 0x0026144C
 bool m_match;	// 0x08
};
// SIZE 0x0C
