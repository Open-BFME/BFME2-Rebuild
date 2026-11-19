// DualWeaponBehaviorModuleData.h: the INI data of DualWeaponBehavior. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x0045AE25; buildFieldParse RVA 0x0045AE44 adds VA 0x00C41858 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DUAL_WEAPON_BEHAVIOR_MODULE_DATA_H
#define BFME2_DUAL_WEAPON_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class DualWeaponBehaviorModuleData : public ModuleData
{
public:
	DualWeaponBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_switchWeaponOnCloseRangeDistance;               // +0x8 SwitchWeaponOnCloseRangeDistance
	Bool m_useCloseRangeWhileMounted;                      // +0xC UseCloseRangeWhileMounted
	UnsignedInt m_minimumSwitchTime;                       // +0x10 MinimumSwitchTime; duration, frames
	Bool m_useHordeRangeWeapon;                            // +0x14 UseHordeRangeWeapon
	Bool m_useRealVictimRange;                             // +0x15 UseRealVictimRange
};
// SIZE 0x18
typedef char DualWeaponBehaviorModuleData_size[(sizeof(DualWeaponBehaviorModuleData) == 0x18) ? 1 : -1];
typedef char DualWeaponBehaviorModuleData_off_m_switchWeaponOnCloseRangeDistance[(offsetof(DualWeaponBehaviorModuleData, m_switchWeaponOnCloseRangeDistance) == 0x8) ? 1 : -1];
typedef char DualWeaponBehaviorModuleData_off_m_useCloseRangeWhileMounted[(offsetof(DualWeaponBehaviorModuleData, m_useCloseRangeWhileMounted) == 0xC) ? 1 : -1];
typedef char DualWeaponBehaviorModuleData_off_m_minimumSwitchTime[(offsetof(DualWeaponBehaviorModuleData, m_minimumSwitchTime) == 0x10) ? 1 : -1];
typedef char DualWeaponBehaviorModuleData_off_m_useHordeRangeWeapon[(offsetof(DualWeaponBehaviorModuleData, m_useHordeRangeWeapon) == 0x14) ? 1 : -1];
typedef char DualWeaponBehaviorModuleData_off_m_useRealVictimRange[(offsetof(DualWeaponBehaviorModuleData, m_useRealVictimRange) == 0x15) ? 1 : -1];

#endif
