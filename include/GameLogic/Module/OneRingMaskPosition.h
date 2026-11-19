#pragma once
#include "Common/CreateMask.h"
#include "Common/Coord3D.h"
union OneRingMaskPosition
{
 CreateMask mask;
 struct PositionOverlay { unsigned pad; Coord3DBase pos; } overlay;	// Coord3D has constructors; a union member may not (k02)
};
