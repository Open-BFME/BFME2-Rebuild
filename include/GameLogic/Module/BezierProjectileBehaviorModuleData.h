#pragma once
#include "GameLogic/Module/ModuleData.h"
class BezierProjectileBehaviorModuleData : public ModuleData
{
public:
 BezierProjectileBehaviorModuleData();
 virtual ~BezierProjectileBehaviorModuleData();
 static void buildFieldParse(MultiIniFieldParse &);
 unsigned char m_unk0x08[0xBC];
};
// SIZE 0xC4
