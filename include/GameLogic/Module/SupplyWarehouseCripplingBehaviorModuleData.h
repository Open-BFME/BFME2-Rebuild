// SupplyWarehouseCripplingBehaviorModuleData.h: the INI data of SupplyWarehouseCripplingBehavior. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x00483A61; buildFieldParse RVA 0x00483A7A adds VA 0x00C49B40 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SUPPLY_WAREHOUSE_CRIPPLING_BEHAVIOR_MODULE_DATA_H
#define BFME2_SUPPLY_WAREHOUSE_CRIPPLING_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class SupplyWarehouseCripplingBehaviorModuleData : public ModuleData
{
public:
	SupplyWarehouseCripplingBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_selfHealSupression;                      // +0x8 SelfHealSupression; duration, frames
	UnsignedInt m_selfHealDelay;                           // +0xC SelfHealDelay; duration, frames
	Real m_selfHealAmount;                                 // +0x10 SelfHealAmount
};
// SIZE 0x14
typedef char SupplyWarehouseCripplingBehaviorModuleData_size[(sizeof(SupplyWarehouseCripplingBehaviorModuleData) == 0x14) ? 1 : -1];
typedef char SupplyWarehouseCripplingBehaviorModuleData_off_m_selfHealSupression[(offsetof(SupplyWarehouseCripplingBehaviorModuleData, m_selfHealSupression) == 0x8) ? 1 : -1];
typedef char SupplyWarehouseCripplingBehaviorModuleData_off_m_selfHealDelay[(offsetof(SupplyWarehouseCripplingBehaviorModuleData, m_selfHealDelay) == 0xC) ? 1 : -1];
typedef char SupplyWarehouseCripplingBehaviorModuleData_off_m_selfHealAmount[(offsetof(SupplyWarehouseCripplingBehaviorModuleData, m_selfHealAmount) == 0x10) ? 1 : -1];

#endif
