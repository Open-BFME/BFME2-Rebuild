// ReplaceSelfUpgradeModuleData.h: the INI data of ReplaceSelfUpgrade. SIZE 0x124.
// Target facts: ModuleFactory registers the module with operator new 0x124 and
// constructor RVA 0x004B790B; buildFieldParse RVA 0x004B7023 adds VA 0x00C58A88 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_replaceWith +0x118: INI::parseAsciiStringVectorAppend 0x0002E896
#pragma once
#ifndef BFME2_REPLACE_SELF_UPGRADE_MODULE_DATA_H
#define BFME2_REPLACE_SELF_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"

class INI;
class MultiIniFieldParse;

class ReplaceSelfUpgradeModuleData : public ModuleData
{
public:
	ReplaceSelfUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	UnsignedByte m_replaceWith[0xC];                       // +0x118 ReplaceWith (also AndThenAddA); unresolved: INI::parseAsciiStringVectorAppend
};
// SIZE 0x124
typedef char ReplaceSelfUpgradeModuleData_size[(sizeof(ReplaceSelfUpgradeModuleData) == 0x124) ? 1 : -1];
typedef char ReplaceSelfUpgradeModuleData_off_m_upgradeMuxData[(offsetof(ReplaceSelfUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char ReplaceSelfUpgradeModuleData_off_m_replaceWith[(offsetof(ReplaceSelfUpgradeModuleData, m_replaceWith) == 0x118) ? 1 : -1];

#endif
