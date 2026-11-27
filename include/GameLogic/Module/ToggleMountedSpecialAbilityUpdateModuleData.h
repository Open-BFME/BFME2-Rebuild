// The mounted template, opacity and timer synchronization settings.
#pragma once
#ifndef BFME2_TOGGLE_MOUNTED_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_TOGGLE_MOUNTED_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char ToggleMountedSpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class ToggleMountedSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	ToggleMountedSpecialAbilityUpdateModuleData();
	virtual ~ToggleMountedSpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_opacityTarget;                                  // +0xC8 OpacityTarget
	Bool m_triggerInstantlyOnCreate;                       // +0xCC TriggerInstantlyOnCreate
	Bool m_cancelDisguiseWhenDismounting;                  // +0xCD CancelDisguiseWhenDismounting
	AsciiString m_mountedTemplate;                         // +0xD0 MountedTemplate
	_STL::vector<AsciiString> m_synchronizeTimerOnSpecialPower;// +0xD4 SynchronizeTimerOnSpecialPower
};
// SIZE 0xE0
typedef char ToggleMountedSpecialAbilityUpdateModuleData_size[(sizeof(ToggleMountedSpecialAbilityUpdateModuleData) == 0xE0) ? 1 : -1];
typedef char ToggleMountedSpecialAbilityUpdateModuleData_off_m_opacityTarget[(offsetof(ToggleMountedSpecialAbilityUpdateModuleData, m_opacityTarget) == 0xC8) ? 1 : -1];
typedef char ToggleMountedSpecialAbilityUpdateModuleData_off_m_triggerInstantlyOnCreate[(offsetof(ToggleMountedSpecialAbilityUpdateModuleData, m_triggerInstantlyOnCreate) == 0xCC) ? 1 : -1];
typedef char ToggleMountedSpecialAbilityUpdateModuleData_off_m_cancelDisguiseWhenDismounting[(offsetof(ToggleMountedSpecialAbilityUpdateModuleData, m_cancelDisguiseWhenDismounting) == 0xCD) ? 1 : -1];
typedef char ToggleMountedSpecialAbilityUpdateModuleData_off_m_mountedTemplate[(offsetof(ToggleMountedSpecialAbilityUpdateModuleData, m_mountedTemplate) == 0xD0) ? 1 : -1];
typedef char ToggleMountedSpecialAbilityUpdateModuleData_off_m_synchronizeTimerOnSpecialPower[(offsetof(ToggleMountedSpecialAbilityUpdateModuleData, m_synchronizeTimerOnSpecialPower) == 0xD4) ? 1 : -1];

#endif
