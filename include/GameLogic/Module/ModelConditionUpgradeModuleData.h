// Promoted from Draft by f03: both flag sets are ModelConditionFlags (the 591-name set, 0x4C bytes, so
// +0x164 + 0x4C meets +0x1B0), and the temporary condition is the int iniParseAnim 0x004B6481 stores (-1 for none).
// ModelConditionUpgradeModuleData.h: the INI data of ModelConditionUpgrade. SIZE 0x1B8.
// Target facts: ModuleFactory registers the module with operator new 0x1B8 and
// constructor RVA 0x004B6428; buildFieldParse RVA 0x004B6691 adds VA 0x00C58730 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_MODEL_CONDITION_UPGRADE_MODULE_DATA_H
#define BFME2_MODEL_CONDITION_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/ModelConditionFlags.h"

class INI;
class MultiIniFieldParse;

class ModelConditionUpgradeModuleData : public ModuleData
{
public:
	ModelConditionUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	ModelConditionFlags m_addConditionFlags;               // +0x118 AddConditionFlags (ModelConditionFlags parse wrapper 0x000B9468)
	ModelConditionFlags m_removeConditionFlags;            // +0x164 RemoveConditionFlags (also RemoveConditionFlagsInRange 0x004B65E4)
	Int m_addTempConditionFlag;                            // +0x1B0 AddTempConditionFlag; INI::iniParseAnim
	Real m_tempConditionTime;                              // +0x1B4 TempConditionTime
};
// SIZE 0x1B8
typedef char ModelConditionUpgradeModuleData_size[(sizeof(ModelConditionUpgradeModuleData) == 0x1B8) ? 1 : -1];
typedef char ModelConditionUpgradeModuleData_off_m_upgradeMuxData[(offsetof(ModelConditionUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char ModelConditionUpgradeModuleData_off_m_addConditionFlags[(offsetof(ModelConditionUpgradeModuleData, m_addConditionFlags) == 0x118) ? 1 : -1];
typedef char ModelConditionUpgradeModuleData_off_m_removeConditionFlags[(offsetof(ModelConditionUpgradeModuleData, m_removeConditionFlags) == 0x164) ? 1 : -1];
typedef char ModelConditionUpgradeModuleData_off_m_addTempConditionFlag[(offsetof(ModelConditionUpgradeModuleData, m_addTempConditionFlag) == 0x1B0) ? 1 : -1];
typedef char ModelConditionUpgradeModuleData_off_m_tempConditionTime[(offsetof(ModelConditionUpgradeModuleData, m_tempConditionTime) == 0x1B4) ? 1 : -1];

#endif
