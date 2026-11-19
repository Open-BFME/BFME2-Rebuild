#pragma once
class Object;
// GameLogic+0x178's manager, named by its retail snapshot string.
class InvisibilityManager
{
public:
 bool detected(Object *,Object *,int,int); // 0x00439E0C
 void rva00439CF7(Object *object,int duration,const void *nugget); // 0x00439CF7 (the map spells the class Rva00439CF7; InvisibilitySpecialPower::doSpecialPowerAtObject 0x004C2419 calls it on TheGameLogic+0x178)
};
