// GrantUpgradeCreateModuleData.h: the INI data of GrantUpgradeCreate. SIZE 0x20.
// Target facts: ModuleFactory registers the module with operator new 0x20 and
// constructor RVA 0x004B90E1; buildFieldParse RVA 0x004B8FF0 adds VA 0x00C594A0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_GRANT_UPGRADE_CREATE_MODULE_DATA_H
#define BFME2_GRANT_UPGRADE_CREATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;

class GrantUpgradeCreateModuleData : public ModuleData
{
public:
	GrantUpgradeCreateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_upgradeToGrant;                          // +0x8 UpgradeToGrant
	ObjectStatusMaskType m_exemptStatus;                   // +0xC ExemptStatus; BitFlags<101>
	Bool m_giveOnBuildComplete;                            // +0x1C GiveOnBuildComplete
};
// SIZE 0x20
typedef char GrantUpgradeCreateModuleData_size[(sizeof(GrantUpgradeCreateModuleData) == 0x20) ? 1 : -1];
typedef char GrantUpgradeCreateModuleData_off_m_upgradeToGrant[(offsetof(GrantUpgradeCreateModuleData, m_upgradeToGrant) == 0x8) ? 1 : -1];
typedef char GrantUpgradeCreateModuleData_off_m_exemptStatus[(offsetof(GrantUpgradeCreateModuleData, m_exemptStatus) == 0xC) ? 1 : -1];
typedef char GrantUpgradeCreateModuleData_off_m_giveOnBuildComplete[(offsetof(GrantUpgradeCreateModuleData, m_giveOnBuildComplete) == 0x1C) ? 1 : -1];

#endif
