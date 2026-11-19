#pragma once
#include "GameLogic/TerrainLogic.h"
// TerrainLogic has four bases, so its own member pointers are 8 bytes; the table
// is read through its first base (Snapshot, at +0), whose member pointers are 4.
typedef float (Snapshot::*GroundHeightMember)(float,float,Coord3D *) const;
typedef GroundHeightMember TerrainGroundMethods[7];
typedef char GroundHeightMemberWidth[sizeof(GroundHeightMember)==4?1:-1];
