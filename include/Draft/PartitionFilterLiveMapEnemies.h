// DRAFT (h11): not proven by a matching caller yet. Name: Zero Hour's AI.cpp-local
// PartitionFilterLiveMapEnemies (evidence for the name and shape only).
// Target facts: vtable VA 0x00C071B4 (RVA 0x008071B4, 3 slots: shared dtor 0x00395A19, allow
// 0x002FE13D, getPlayerMask 0x002FE108); +8 the object whose enemies are wanted (constructed inline
// in AI::rva002FEEAD 0x002FEEAD, ctor call 0x002FD66B region). allow: false when the candidate's
// private status byte (+0x438) has bit 0 (effectively dead) or bit 3 (off the map), when its
// +0x25C helper's 0x00390533 is true, or when its template flags +0x10E bit 6 / +0x115 bit 5 are
// set; otherwise true when m_obj->getRelationship(candidate) is 0 (enemies).
#pragma once
#include "GameLogic/PartitionFilter.h"
class PartitionFilterLiveMapEnemies : public PartitionFilter
{
public:
 PartitionFilterLiveMapEnemies(const Object *obj) : m_obj(obj) {}
 virtual bool allow(Object *);
 virtual int getPlayerMask();	// 0x002FE108
 const Object *m_obj;	// 0x08
};
// SIZE 0x0C
