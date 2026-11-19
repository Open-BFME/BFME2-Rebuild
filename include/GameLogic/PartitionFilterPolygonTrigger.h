#pragma once
#include "GameLogic/PartitionFilter.h"
class PolygonTrigger;
// Zero Hour's PartitionFilterPolygonTrigger: accepts the objects inside a polygon trigger (compared on x and y only).
// Target facts: allow 0x00261723 truncates the candidate's x and y (cvttss2si) into an integer position and asks the
// trigger at +8 (pointInTrigger 0x002E3A13).
class PartitionFilterPolygonTrigger : public PartitionFilter
{
public:
 PartitionFilterPolygonTrigger(PolygonTrigger *trigger) : m_trigger(trigger) {}
 virtual bool allow(Object *);	// 0x00261723
 PolygonTrigger *m_trigger;	// 0x08
};
// SIZE 0x0C
