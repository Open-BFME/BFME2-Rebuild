#pragma once
#include "GameLogic/PartitionFilter.h"
// A filter on whether the candidate has a script name (ScriptEngine::rva00357AF8: a named object, or an unnamed one whose
// id is in the scoped object-id table): accepts the objects for which that equals m_match. Retail's class name is
// unknown (Zero Hour has no such filter); the role name is the behaviour. Target facts: vtable 0x0081FE10, allow
// 0x0026149C.
class PartitionFilterScriptNamedObject : public PartitionFilter
{
public:
 PartitionFilterScriptNamedObject(bool match) : m_match(match) {}
 virtual bool allow(Object *);	// 0x0026149C
 bool m_match;	// 0x08
};
// SIZE 0x0C
