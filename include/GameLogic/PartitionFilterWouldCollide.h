#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/Coord3D.h"
class GeometryInfo;
// Zero Hour's PartitionFilterWouldCollide: accepts the objects that collide (or, desired false, do not collide) with a
// geometry placed at a position and angle. Target facts: allow 0x00261603 hands the stored position (+8), height/angle
// (+0x18) and geometry (+0x14) and the candidate's position, geometry and angle to GeometryInfo::CollidesWith
// 0x006BEB80 and compares the answer with the byte at +0x1C. BFME 2 keeps the geometry by pointer.
class PartitionFilterWouldCollide : public PartitionFilter
{
public:
 PartitionFilterWouldCollide(const Coord3D &pos, const GeometryInfo *geom, float angle, bool desiredCollisionResult)
  : m_position(pos), m_geom(geom), m_angle(angle), m_desiredCollisionResult(desiredCollisionResult) {}
 virtual bool allow(Object *);	// 0x00261603
 Coord3D m_position;	// 0x08
 const GeometryInfo *m_geom;	// 0x14
 float m_angle;	// 0x18
 bool m_desiredCollisionResult;	// 0x1C
};
// SIZE 0x20
