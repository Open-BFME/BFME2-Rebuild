// StatusBitsUpgradeModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x004B493E);
// name, size, bases and virtual order are retail facts given below.
// StatusBitsUpgradeModuleData.h: the INI data of StatusBitsUpgrade. SIZE 0x138.
// Target facts: ModuleFactory registers the module with operator new 0x138 and
// constructor RVA 0x00254775; buildFieldParse RVA 0x004B493E adds VA 0x00C57984 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_STATUS_BITS_UPGRADE_MODULE_DATA_H
#define BFME2_STATUS_BITS_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;

// Derives from UpgradeModuleData (0x118, the upgrade trigger data at +0x8), as every upgrade
// module data does; holding UpgradeMuxData as a member here gave a wrong constructor.
class StatusBitsUpgradeModuleData : public UpgradeModuleData
{
public:
	StatusBitsUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);				// 0x004B493E

	ObjectStatusMaskType m_statusToSet;                    // +0x118 StatusToSet; BitFlags<101>
	ObjectStatusMaskType m_statusToClear;                  // +0x128 StatusToClear; BitFlags<101>
};
// SIZE 0x138
typedef char StatusBitsUpgradeModuleData_size[(sizeof(StatusBitsUpgradeModuleData) == 0x138) ? 1 : -1];
typedef char StatusBitsUpgradeModuleData_off_m_statusToSet[(offsetof(StatusBitsUpgradeModuleData, m_statusToSet) == 0x118) ? 1 : -1];
typedef char StatusBitsUpgradeModuleData_off_m_statusToClear[(offsetof(StatusBitsUpgradeModuleData, m_statusToClear) == 0x128) ? 1 : -1];

#endif
