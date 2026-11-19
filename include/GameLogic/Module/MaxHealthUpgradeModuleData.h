// MaxHealthUpgradeModuleData.h: the INI data of MaxHealthUpgrade. SIZE 0x120.
// UpgradeModuleData (0x118) plus AddMaxHealth at +0x118 and the change type at +0x11C.
// The constructor 0x004B62C4 calls UpgradeModuleData's constructor, stores its own vtable,
// then clears AddMaxHealth and sets the change type to 0 (the first MaxHealthChangeType value,
// SAME_CURRENTHEALTH). buildFieldParse (0x004B62E8) chains UpgradeMuxData's table at +0x8, then its own.
#pragma once
#ifndef BFME2_MAX_HEALTH_UPGRADE_MODULE_DATA_H
#define BFME2_MAX_HEALTH_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class MultiIniFieldParse;

class MaxHealthUpgradeModuleData : public UpgradeModuleData
{
public:
	MaxHealthUpgradeModuleData();										// 0x004B62C4
	static void buildFieldParse(MultiIniFieldParse &parse);				// 0x004B62E8

	Real m_addMaxHealth;												// +0x118 AddMaxHealth
	Int m_changeType;													// +0x11C ChangeType (MaxHealthChangeType)
};
typedef char MaxHealthUpgradeModuleData_size[(sizeof(MaxHealthUpgradeModuleData) == 0x120) ? 1 : -1];
typedef char MaxHealthUpgradeModuleData_off_m_addMaxHealth[(offsetof(MaxHealthUpgradeModuleData, m_addMaxHealth) == 0x118) ? 1 : -1];
typedef char MaxHealthUpgradeModuleData_off_m_changeType[(offsetof(MaxHealthUpgradeModuleData, m_changeType) == 0x11C) ? 1 : -1];

#endif
