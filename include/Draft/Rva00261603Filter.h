// Draft/Rva00261603Filter.h: the partition filter FoundationAIUpdate::rva00455BDD 0x00455BDD builds inline on its stack.
// Target facts: constructor 0x0027C2C9 (ledger name ??0Rva00261603Filter, arguments (const Coord3D &, const GeometryInfo &, Real, Bool),
// out of line) stores VA 0x00BFB1B8 at +0, zeroes +4 (the chain link), copies the position to +8..+0x13, the geometry pointer to +0x14,
// the Real to +0x18 and the Bool to +0x1C: SIZE 0x20. allow 0x00261603 passes the object's position, geometry (+0xA8) and
// radius (+0x44) with this filter's members to 0x006BEB80 and compares the result with the Bool. Roles are not established.
#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/Coord3D.h"
class GeometryInfo;
class Rva00261603Filter : public PartitionFilter
{
public:
  Rva00261603Filter(const Coord3D &position, const GeometryInfo &geometry, Real angle, Bool flag); // 0x0027C2C9
  virtual bool allow(Object *object); // 0x00261603
  Coord3D m_position; // +0x08
  const GeometryInfo *m_geometry; // +0x14
  Real m_angle; // +0x18
  Bool m_flag; // +0x1C
};
// SIZE 0x20
