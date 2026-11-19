#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/GameType.h"
class Coord3D;
// Zero Hour's PartitionFilterIrregularArea: accepts the objects inside a polygon (their x and y against its points).
// Target facts: allow 0x0026170A (MAE, protected) hands the candidate's position (Thing +0x38), the point array at +8
// and the count at +0xC to PointInsideArea2D 0x0025F86D.
class PartitionFilterIrregularArea : public PartitionFilter
{
public:
 PartitionFilterIrregularArea(Coord3D *area, Int numPointsInArea) : m_area(area), m_numPointsInArea(numPointsInArea) {}
protected:
 virtual bool allow(Object *);	// 0x0026170A
public:
 Coord3D *m_area;	// 0x08
 Int m_numPointsInArea;	// 0x0C
};
// SIZE 0x10
