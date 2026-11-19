#pragma once
class Object;
class Coord3D;
struct Rva0045B936Arg;
class Rva002CBA7C;
class Matrix3D;
class ProjectileUpdateInterface
{
public:
 virtual void projectileLaunchAtObjectOrPosition(Object *,const Coord3D *,Object *,int,int,const Rva0045B936Arg *,Rva002CBA7C *,const Matrix3D *)=0;
 virtual unsigned char rva004A768A() const=0;
 virtual int getLauncherID() const=0;
 virtual bool rva0045CD6E(Object *)=0;
 virtual bool wouldLikeToCollideWith(const Object *other) const = 0;
 virtual void rva0045B4E2(int)=0;
};
