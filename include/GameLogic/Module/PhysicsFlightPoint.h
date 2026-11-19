#pragma once
#include "Common/Coord3D.h"
// Flight control-point arrays use the nontrivial Coord3D lifetime shape.
struct PhysicsFlightPoint : Coord3D
{
 PhysicsFlightPoint();
 ~PhysicsFlightPoint();
};
// SIZE 0xC
