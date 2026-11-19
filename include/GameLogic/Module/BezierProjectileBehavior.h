#pragma once
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ProjectileUpdateInterface.h"
#include "GameLogic/Module/CollideModule.h"
class BezierProjectileBehavior : public UpdateModule,public ProjectileUpdateInterface,public CollideModuleInterface
{
public:
 BezierProjectileBehavior(Thing *,const ModuleData *);
 virtual ~BezierProjectileBehavior();
 virtual UpdateSleepTime update();
 virtual void projectileLaunchAtObjectOrPosition(Object *,const Coord3D *,Object *,int,int,const Rva0045B936Arg *,Rva002CBA7C *,const Matrix3D *);
 virtual bool rva0045CD6E(Object *);
 virtual CollideModuleInterface *getCollide() { return this; }
 virtual ProjectileUpdateInterface *getProjectileUpdateInterface() { return this; }
 virtual void onCollide(Object *other, const Coord3D *location, const Coord3D *normal);
 virtual Bool wouldLikeToCollideWith(const Object *other) const;
 virtual Bool isHijackedVehicleCrateCollide() const { return false; }
 virtual Bool isCarBombCrateCollide() const { return false; }
 virtual Bool isRailroad() const { return false; }
 virtual Bool isSalvageCrateCollide() const { return false; }
 virtual void projectileFireAtObjectOrPosition(Object *,const Coord3D *);
 void rva0045C026();
protected:
 virtual void xfer(Xfer *);
 unsigned char m_unk0x28[0x18];
 int m_40; // 0x40
 unsigned char m_unk0x44[0x44];
};
// SIZE 0x88
