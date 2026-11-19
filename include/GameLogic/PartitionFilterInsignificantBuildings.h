// Zero Hour's PartitionFilterInsignificantBuildings (h11's header; its allow row 0x00261246 now proves the shape and
// the member names, which are Zero Hour's).
// Target facts: vtable RVA 0x00807160 (3 slots: shared deleting dtor 0x00395A19,
// allow 0x00261246, the default getPlayerMask 0x0036CC7A); constructor stores 0 at +4 (link) and
// two bytes at +8/+9 (inline in AI::rva002FEEAD 0x002FEEAD, ctor call 0x002FD681).
// allow: a non-structure candidate (Object::isStructure 0x0028D481 false) returns the byte at +8;
// a structure that is a non-faction structure (0x0028D4C4), with the byte at +9 clear, no object at
// +0x258 and a +0x250 interface whose slot 4 (+0x10) is true and slot 69 (+0x114, arg 0) is zero,
// is rejected; every other structure is accepted.
#pragma once
#include "GameLogic/PartitionFilter.h"
class PartitionFilterInsignificantBuildings : public PartitionFilter
{
public:
 PartitionFilterInsignificantBuildings(bool allowNonBuildings, bool allowInsignificant)
  : m_allowNonBuildings(allowNonBuildings), m_allowInsignificant(allowInsignificant) {}
 virtual bool allow(Object *);
 bool m_allowNonBuildings;	// 0x08
 bool m_allowInsignificant;	// 0x09
};
// SIZE 0x0C
