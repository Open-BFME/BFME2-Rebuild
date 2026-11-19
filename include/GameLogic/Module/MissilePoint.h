#pragma once
#include "Common/Coord3D.h"
struct MissilePoint : public Coord3D
{
 MissilePoint(const Coord3D &p) { x=p.x;y=p.y;z=p.z; }
 void setXY(const Coord3D &p,float height) { x=p.x;y=p.y;z=height; }
};
