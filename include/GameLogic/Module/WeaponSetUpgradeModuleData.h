// WeaponSetUpgradeModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x004B56F6);
// name, size, bases and virtual order are retail facts given below.
// WeaponSetUpgradeModuleData.h: the INI data of WeaponSetUpgrade. SIZE 0x128.
// Target facts: ModuleFactory registers the module with operator new 0x128 and
// constructor RVA 0x004B57AA; buildFieldParse RVA 0x004B56F6 adds VA 0x00C58020 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_WEAPON_SET_UPGRADE_MODULE_DATA_H
#define BFME2_WEAPON_SET_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;

// Derives from UpgradeModuleData (0x118, the upgrade trigger data at +0x8), as every upgrade
// module data does; holding UpgradeMuxData as a member here gave a wrong constructor.
class WeaponSetUpgradeModuleData : public UpgradeModuleData
{
public:
	WeaponSetUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);				// 0x004B56F6

	WeaponSetFlags m_weaponCondition;                      // +0x118 WeaponCondition; BitFlags<104>
};
// SIZE 0x128
typedef char WeaponSetUpgradeModuleData_size[(sizeof(WeaponSetUpgradeModuleData) == 0x128) ? 1 : -1];
typedef char WeaponSetUpgradeModuleData_off_m_weaponCondition[(offsetof(WeaponSetUpgradeModuleData, m_weaponCondition) == 0x118) ? 1 : -1];

#endif
