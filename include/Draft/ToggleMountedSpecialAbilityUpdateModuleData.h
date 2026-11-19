// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BEF770).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ToggleMountedSpecialAbilityUpdateModuleData.h: the INI data of ToggleMountedSpecialAbilityUpdate. SIZE 0xE0.
// Target facts: ModuleFactory registers the module with operator new 0xE0 and
// constructor RVA 0x004AE0AC; buildFieldParse RVA 0x0024F66F calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEF770 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
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
	static void buildFieldParse(MultiIniFieldParse &parse);

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
