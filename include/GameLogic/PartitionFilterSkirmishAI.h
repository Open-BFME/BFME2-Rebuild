#pragma once
#include "GameLogic/PartitionFilter.h"
class SkirmishAI;
// The skirmish AI's attack-move target filter: allow asks the brain's TacticalAI whether the candidate is an
// enemy worth an attack move for m_obj (only for objects whose AI update kind is 0x21).
class PartitionFilterSkirmishAI : public PartitionFilter
{
public:
 virtual bool allow(Object *);
 Object *m_obj; // 0x08
 SkirmishAI *m_ai; // 0x0C
};
// SIZE 0x10
