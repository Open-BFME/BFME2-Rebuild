#pragma once
#include "GameLogic/Module/BezierProjectileBehaviorModuleData.h"
#include "Common/BitFlags.h"
class FXList;
class MissileUpdateModuleData : public BezierProjectileBehaviorModuleData
{
public:
 MissileUpdateModuleData();
 virtual ~MissileUpdateModuleData();
 static void buildFieldParse(MultiIniFieldParse &);
 int m_fuelLifetime; // 0xC4
 int m_ignitionDelay; // 0xC8
 float m_unkCC; // 0xCC
 float m_distanceToTargetBeforeDiving; // 0xD0
 const FXList *m_ignitionFX; // 0xD4
 unsigned char m_detonateOnNoFuel; // 0xD8
 int m_unkDC; // 0xDC
 KindOfMaskType m_garrisonHitKillRequiredKindOf; // 0xE0
 KindOfMaskType m_garrisonHitKillForbiddenKindOf; // 0xFC
 int m_garrisonHitKillFX; // 0x118
 AsciiString m_exhaust; // 0x11C
};
// SIZE 0x120
