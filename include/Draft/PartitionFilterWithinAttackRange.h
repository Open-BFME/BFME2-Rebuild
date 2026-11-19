// DRAFT (h11): not proven by a matching caller yet. Name: Zero Hour's AI.cpp-local
// PartitionFilterWithinAttackRange (evidence for the name and shape only).
// Target facts: vtable RVA 0x008071C0 (3 slots: shared dtor 0x00395A19, allow 0x002FE371, the
// default getPlayerMask 0x0036CC7A). allow forwards (m_obj, candidate) to the static
// AI::rva002FE193 (0x002FE193, bool(Object *, Object *)): the candidate is within m_obj's attack range.
#pragma once
#include "GameLogic/PartitionFilter.h"
class PartitionFilterWithinAttackRange : public PartitionFilter
{
public:
 PartitionFilterWithinAttackRange(const Object *obj) : m_obj(obj) {}
 virtual bool allow(Object *);
 const Object *m_obj;	// 0x08
};
// SIZE 0x0C
