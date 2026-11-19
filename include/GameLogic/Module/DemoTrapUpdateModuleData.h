// DemoTrapUpdateModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x00495AA9);
// name, size, bases and virtual order are retail facts given below.
// DemoTrapUpdateModuleData.h: the INI data of DemoTrapUpdate. SIZE 0x40.
// Target facts: ModuleFactory registers the module with operator new 0x40 and
// constructor RVA 0x0049597A; buildFieldParse RVA 0x00495AA9 adds VA 0x00C4EEC8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DEMO_TRAP_UPDATE_MODULE_DATA_H
#define BFME2_DEMO_TRAP_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class WeaponTemplate;

class DemoTrapUpdateModuleData : public ModuleData
{
public:
	DemoTrapUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const WeaponTemplate * m_detonationWeapon;             // +0x8 DetonationWeapon
	KindOfMaskType m_ignoreTargetTypes;                    // +0xC IgnoreTargetTypes; BitFlags<218>
	Int m_manualModeWeaponSlot;                            // +0x28 ManualModeWeaponSlot; lookup-list value (enum)
	Int m_detonationWeaponSlot;                            // +0x2C DetonationWeaponSlot; lookup-list value (enum)
	Int m_proximityModeWeaponSlot;                         // +0x30 ProximityModeWeaponSlot; lookup-list value (enum)
	Real m_triggerDetonationRange;                         // +0x34 TriggerDetonationRange
	UnsignedInt m_scanRate;                                // +0x38 ScanRate; duration, frames
	Bool m_defaultProximityMode;                           // +0x3C DefaultProximityMode
	Bool m_autoDetonationWithFriendsInvolved;              // +0x3D AutoDetonationWithFriendsInvolved
	Bool m_detonateWhenKilled;                             // +0x3E DetonateWhenKilled
};
// SIZE 0x40
typedef char DemoTrapUpdateModuleData_size[(sizeof(DemoTrapUpdateModuleData) == 0x40) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_detonationWeapon[(offsetof(DemoTrapUpdateModuleData, m_detonationWeapon) == 0x8) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_ignoreTargetTypes[(offsetof(DemoTrapUpdateModuleData, m_ignoreTargetTypes) == 0xC) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_manualModeWeaponSlot[(offsetof(DemoTrapUpdateModuleData, m_manualModeWeaponSlot) == 0x28) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_detonationWeaponSlot[(offsetof(DemoTrapUpdateModuleData, m_detonationWeaponSlot) == 0x2C) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_proximityModeWeaponSlot[(offsetof(DemoTrapUpdateModuleData, m_proximityModeWeaponSlot) == 0x30) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_triggerDetonationRange[(offsetof(DemoTrapUpdateModuleData, m_triggerDetonationRange) == 0x34) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_scanRate[(offsetof(DemoTrapUpdateModuleData, m_scanRate) == 0x38) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_defaultProximityMode[(offsetof(DemoTrapUpdateModuleData, m_defaultProximityMode) == 0x3C) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_autoDetonationWithFriendsInvolved[(offsetof(DemoTrapUpdateModuleData, m_autoDetonationWithFriendsInvolved) == 0x3D) ? 1 : -1];
typedef char DemoTrapUpdateModuleData_off_m_detonateWhenKilled[(offsetof(DemoTrapUpdateModuleData, m_detonateWhenKilled) == 0x3E) ? 1 : -1];

#endif
