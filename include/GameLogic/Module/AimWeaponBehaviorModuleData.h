// AimWeaponBehaviorModuleData.h: the INI data of AimWeaponBehavior. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x0045B1DD; buildFieldParse RVA 0x0045B215 adds VA 0x00C41948 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_AIM_WEAPON_BEHAVIOR_MODULE_DATA_H
#define BFME2_AIM_WEAPON_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class AimWeaponBehaviorModuleData : public ModuleData
{
public:
	AimWeaponBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_aimLowThreshold;                                // +0x8 AimLowThreshold
	Real m_aimHighThreshold;                               // +0xC AimHighThreshold
	Real m_aimNearDistance;                                // +0x10 AimNearDistance
	Real m_aimFarDistance;                                 // +0x14 AimFarDistance
};
// SIZE 0x18
typedef char AimWeaponBehaviorModuleData_size[(sizeof(AimWeaponBehaviorModuleData) == 0x18) ? 1 : -1];
typedef char AimWeaponBehaviorModuleData_off_m_aimLowThreshold[(offsetof(AimWeaponBehaviorModuleData, m_aimLowThreshold) == 0x8) ? 1 : -1];
typedef char AimWeaponBehaviorModuleData_off_m_aimHighThreshold[(offsetof(AimWeaponBehaviorModuleData, m_aimHighThreshold) == 0xC) ? 1 : -1];
typedef char AimWeaponBehaviorModuleData_off_m_aimNearDistance[(offsetof(AimWeaponBehaviorModuleData, m_aimNearDistance) == 0x10) ? 1 : -1];
typedef char AimWeaponBehaviorModuleData_off_m_aimFarDistance[(offsetof(AimWeaponBehaviorModuleData, m_aimFarDistance) == 0x14) ? 1 : -1];

#endif
