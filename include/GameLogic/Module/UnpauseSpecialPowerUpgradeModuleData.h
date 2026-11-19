// UnpauseSpecialPowerUpgradeModuleData.h: the INI data of UnpauseSpecialPowerUpgrade. SIZE 0x120.
// UpgradeModuleData (0x118) plus the special-power template at +0x118 and the no-recharge flag at +0x11C.
// The constructor 0x004B544A calls UpgradeModuleData's constructor first, then clears both fields.
// buildFieldParse (0x004B546A) chains UpgradeMuxData's table at +0x8, then its own table (VA 0x00C57EF0).
#pragma once
#ifndef BFME2_UNPAUSE_SPECIAL_POWER_UPGRADE_MODULE_DATA_H
#define BFME2_UNPAUSE_SPECIAL_POWER_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class SpecialPowerTemplate;

class UnpauseSpecialPowerUpgradeModuleData : public UpgradeModuleData
{
public:
	UnpauseSpecialPowerUpgradeModuleData();							// 0x004B544A
	static void buildFieldParse(MultiIniFieldParse &parse);			// 0x004B546A

	const SpecialPowerTemplate *m_specialPowerTemplate;				// +0x118 SpecialPowerTemplate
	Bool m_obeyRechargeOnTrigger;									// +0x11C ObeyRechageOnTrigger (INI token spelling)
};
typedef char UnpauseSpecialPowerUpgradeModuleData_size[(sizeof(UnpauseSpecialPowerUpgradeModuleData) == 0x120) ? 1 : -1];
typedef char UnpauseSpecialPowerUpgradeModuleData_off_m_specialPowerTemplate[(offsetof(UnpauseSpecialPowerUpgradeModuleData, m_specialPowerTemplate) == 0x118) ? 1 : -1];
typedef char UnpauseSpecialPowerUpgradeModuleData_off_m_obeyRechargeOnTrigger[(offsetof(UnpauseSpecialPowerUpgradeModuleData, m_obeyRechargeOnTrigger) == 0x11C) ? 1 : -1];

#endif
