// ShareExperienceBehaviorModuleData.h: the INI data of ShareExperienceBehavior. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x0047FE90; buildFieldParse RVA 0x0047FEDC adds VA 0x00C48740 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SHARE_EXPERIENCE_BEHAVIOR_MODULE_DATA_H
#define BFME2_SHARE_EXPERIENCE_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class ShareExperienceBehaviorModuleData : public ModuleData
{
public:
	ShareExperienceBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_radius;                                         // +0x8 Radius
	Real m_dropOff;                                        // +0xC DropOff
	Real m_percentage;                                     // +0x10 Percentage
	ObjectFilter m_objectFilter;                           // +0x14 ObjectFilter
};
// SIZE 0x18
typedef char ShareExperienceBehaviorModuleData_size[(sizeof(ShareExperienceBehaviorModuleData) == 0x18) ? 1 : -1];
typedef char ShareExperienceBehaviorModuleData_off_m_radius[(offsetof(ShareExperienceBehaviorModuleData, m_radius) == 0x8) ? 1 : -1];
typedef char ShareExperienceBehaviorModuleData_off_m_dropOff[(offsetof(ShareExperienceBehaviorModuleData, m_dropOff) == 0xC) ? 1 : -1];
typedef char ShareExperienceBehaviorModuleData_off_m_percentage[(offsetof(ShareExperienceBehaviorModuleData, m_percentage) == 0x10) ? 1 : -1];
typedef char ShareExperienceBehaviorModuleData_off_m_objectFilter[(offsetof(ShareExperienceBehaviorModuleData, m_objectFilter) == 0x14) ? 1 : -1];

#endif
