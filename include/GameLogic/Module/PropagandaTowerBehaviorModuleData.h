// PropagandaTowerBehaviorModuleData.h: the INI data of PropagandaTowerBehavior. SIZE 0x24.
// Target facts: ModuleFactory registers the module with operator new 0x24 and
// constructor RVA 0x0048197C; buildFieldParse RVA 0x004819BE adds VA 0x00C49368 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_PROPAGANDA_TOWER_BEHAVIOR_MODULE_DATA_H
#define BFME2_PROPAGANDA_TOWER_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;

class PropagandaTowerBehaviorModuleData : public ModuleData
{
public:
	PropagandaTowerBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_radius;                                         // +0x8 Radius
	UnsignedInt m_delayBetweenUpdates;                     // +0xC DelayBetweenUpdates; duration, frames
	Real m_healPercentEachSecond;                          // +0x10 HealPercentEachSecond; percent
	const FXList * m_pulseFX;                              // +0x14 PulseFX
	AsciiString m_upgradeRequired;                         // +0x18 UpgradeRequired
	Real m_upgradedHealPercentEachSecond;                  // +0x1C UpgradedHealPercentEachSecond; percent
	const FXList * m_upgradedPulseFX;                      // +0x20 UpgradedPulseFX
};
// SIZE 0x24
typedef char PropagandaTowerBehaviorModuleData_size[(sizeof(PropagandaTowerBehaviorModuleData) == 0x24) ? 1 : -1];
typedef char PropagandaTowerBehaviorModuleData_off_m_radius[(offsetof(PropagandaTowerBehaviorModuleData, m_radius) == 0x8) ? 1 : -1];
typedef char PropagandaTowerBehaviorModuleData_off_m_delayBetweenUpdates[(offsetof(PropagandaTowerBehaviorModuleData, m_delayBetweenUpdates) == 0xC) ? 1 : -1];
typedef char PropagandaTowerBehaviorModuleData_off_m_healPercentEachSecond[(offsetof(PropagandaTowerBehaviorModuleData, m_healPercentEachSecond) == 0x10) ? 1 : -1];
typedef char PropagandaTowerBehaviorModuleData_off_m_pulseFX[(offsetof(PropagandaTowerBehaviorModuleData, m_pulseFX) == 0x14) ? 1 : -1];
typedef char PropagandaTowerBehaviorModuleData_off_m_upgradeRequired[(offsetof(PropagandaTowerBehaviorModuleData, m_upgradeRequired) == 0x18) ? 1 : -1];
typedef char PropagandaTowerBehaviorModuleData_off_m_upgradedHealPercentEachSecond[(offsetof(PropagandaTowerBehaviorModuleData, m_upgradedHealPercentEachSecond) == 0x1C) ? 1 : -1];
typedef char PropagandaTowerBehaviorModuleData_off_m_upgradedPulseFX[(offsetof(PropagandaTowerBehaviorModuleData, m_upgradedPulseFX) == 0x20) ? 1 : -1];

#endif
