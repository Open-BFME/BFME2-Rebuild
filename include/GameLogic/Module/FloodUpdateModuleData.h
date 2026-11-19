#pragma once
#include "GameLogic/Module/ModuleData.h"
#include "Common/StlList.h"
class FloodUpdateModuleData : public ModuleData
{
public:
 FloodUpdateModuleData();
 virtual ~FloodUpdateModuleData();
 static void buildFieldParse(MultiIniFieldParse &);
 _STL::list<int> m_members; // 0x08
 float m_angleOfFlow; // 0x0C
 bool m_directionIsRelative; // 0x10
};
// SIZE 0x14
