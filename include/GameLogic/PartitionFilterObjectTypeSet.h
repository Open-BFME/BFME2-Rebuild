#pragma once
#include "GameLogic/PartitionFilter.h"
class ObjectTypes;
// A filter on the candidate's template name: accepts the objects whose template is in an ObjectTypes set (or, with
// m_match false, the ones that are not). Retail's class name is unknown (Zero Hour has no such filter); the role name is
// the behaviour. Target facts: vtable 0x0081FDF8, allow 0x0026176E asks ObjectTypes::isInSet 0x00376A62 about the
// template's name (Thing +4 -> +0x64) and compares the answer with the byte at +0xC.
class PartitionFilterObjectTypeSet : public PartitionFilter
{
public:
 PartitionFilterObjectTypeSet(const ObjectTypes *types, bool match) : m_types(types), m_match(match) {}
 virtual bool allow(Object *);	// 0x0026176E
 const ObjectTypes *m_types;	// 0x08
 bool m_match;	// 0x0C
};
// SIZE 0x10
