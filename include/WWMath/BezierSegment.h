#pragma once
#include "GameLogic/Module/PhysicsFlightPoint.h"
// The four-point segment copied at 0x0055A246; original type name unknown.
class Rva0055A246
{
public:
 Rva0055A246(const Rva0055A246 &);
 float rva0055A627(float) const;
 void rva0055A7DB(int,void *);
 PhysicsFlightPoint m_points[4]; // 0x00
};
// SIZE 0x30
