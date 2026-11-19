#pragma once
#include "GameLogic/PartitionFilter.h"
class Coord3D;
// Zero Hour's PartitionFilterLineOfSight. vtable 0x00807150 (4 slots), allow
// 0x002619C1: not blocked from m_obj to the candidate (Pathfinder::
// isViewBlockedByObstacle); slot 3 0x0026163A is the same test against a
// position (isAttackViewBlockedByObstacle). m_obj at +0x08.
class PartitionFilterLineOfSight : public PartitionFilter
{
public:
 PartitionFilterLineOfSight(const Object *obj) : m_obj(obj) {}
 virtual bool allow(Object *);
 virtual bool allowPosition(const Coord3D *);	// slot 3, 0x0026163A (name structural)
 const Object *m_obj;	// 0x08
};
// SIZE 0x0C
