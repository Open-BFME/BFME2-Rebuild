#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/BitFlags.h"
// A filter on one kind-of mask: accepts the objects that are any of the kinds (Thing::isAnyKindOf). Retail's class name
// is unknown (Zero Hour has no such filter); the role name is the behaviour. Target facts: allow 0x002610F2 hands the mask
// at +8 to Thing::isAnyKindOf 0x0030ADC7 on the candidate.
class PartitionFilterAnyKindOf : public PartitionFilter
{
public:
 PartitionFilterAnyKindOf(const KindOfMaskType &kinds) : m_kinds(kinds) {}
 virtual bool allow(Object *);	// 0x002610F2
 KindOfMaskType m_kinds;	// 0x08
};
// SIZE 0x24
