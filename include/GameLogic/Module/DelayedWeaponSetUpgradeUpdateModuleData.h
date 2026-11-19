// DelayedWeaponSetUpgradeUpdateModuleData.h: the INI data of DelayedWeaponSetUpgradeUpdate. SIZE 0x8.
// Target facts: ModuleFactory registers the module with operator new 0x8 and
// constructor RVA 0x000B28BF; buildFieldParse RVA 0x004882D3 adds VA 0x00C6BB18 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DELAYED_WEAPON_SET_UPGRADE_UPDATE_MODULE_DATA_H
#define BFME2_DELAYED_WEAPON_SET_UPGRADE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class DelayedWeaponSetUpgradeUpdateModuleData : public ModuleData
{
public:
	DelayedWeaponSetUpgradeUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

};
// SIZE 0x8
typedef char DelayedWeaponSetUpgradeUpdateModuleData_size[(sizeof(DelayedWeaponSetUpgradeUpdateModuleData) == 0x8) ? 1 : -1];

#endif
