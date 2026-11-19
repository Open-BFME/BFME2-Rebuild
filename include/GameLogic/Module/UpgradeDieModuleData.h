// UpgradeDieModuleData.h: the INI data of UpgradeDie. SIZE 0x3C.
// Target facts: ModuleFactory registers the module with operator new 0x3C and
// constructor RVA 0x002550ED; buildFieldParse RVA 0x00253B56 adds VA 0x00BF0958 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_UPGRADE_DIE_MODULE_DATA_H
#define BFME2_UPGRADE_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char UpgradeDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class UpgradeDieModuleData : public DestroyDieModuleData
{
public:
	UpgradeDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_upgradeToRemove;                         // +0x38 UpgradeToRemove
};
// SIZE 0x3C
typedef char UpgradeDieModuleData_size[(sizeof(UpgradeDieModuleData) == 0x3C) ? 1 : -1];
typedef char UpgradeDieModuleData_off_m_upgradeToRemove[(offsetof(UpgradeDieModuleData, m_upgradeToRemove) == 0x38) ? 1 : -1];

#endif
