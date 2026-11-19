// BuildingBehaviorModuleData.h: the INI data of BuildingBehavior. SIZE 0x38.
// Target facts: ModuleFactory registers the module with operator new 0x38 and
// constructor RVA 0x00456489; buildFieldParse RVA 0x00456206 adds VA 0x00C40748 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_BUILDING_BEHAVIOR_MODULE_DATA_H
#define BFME2_BUILDING_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;

class BuildingBehaviorModuleData : public ModuleData
{
public:
	BuildingBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	_STL::vector<AsciiString> m_nightWindowName;           // +0x8 NightWindowName
	_STL::vector<AsciiString> m_fireWindowName;            // +0x14 FireWindowName
	_STL::vector<AsciiString> m_glowWindowName;            // +0x20 GlowWindowName
	_STL::vector<AsciiString> m_fireName;                  // +0x2C FireName
};
// SIZE 0x38
typedef char BuildingBehaviorModuleData_size[(sizeof(BuildingBehaviorModuleData) == 0x38) ? 1 : -1];
typedef char BuildingBehaviorModuleData_off_m_nightWindowName[(offsetof(BuildingBehaviorModuleData, m_nightWindowName) == 0x8) ? 1 : -1];
typedef char BuildingBehaviorModuleData_off_m_fireWindowName[(offsetof(BuildingBehaviorModuleData, m_fireWindowName) == 0x14) ? 1 : -1];
typedef char BuildingBehaviorModuleData_off_m_glowWindowName[(offsetof(BuildingBehaviorModuleData, m_glowWindowName) == 0x20) ? 1 : -1];
typedef char BuildingBehaviorModuleData_off_m_fireName[(offsetof(BuildingBehaviorModuleData, m_fireName) == 0x2C) ? 1 : -1];

#endif
