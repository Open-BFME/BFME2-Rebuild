// RemoveUpgradeUpgradeModuleData.h: the INI data of RemoveUpgradeUpgrade. SIZE 0x134.
// Target facts: ModuleFactory registers the module with operator new 0x134 and
// constructor RVA 0x004B7FF1; buildFieldParse RVA 0x004B7F82 adds VA 0x00C58EA0 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_upgradeToRemove +0x118: INI::parseAsciiStringVectorAppend 0x0002E896
//   m_upgradeGroupsToRemove +0x124: INI::parseAsciiStringVectorAppend 0x0002E896
#pragma once
#ifndef BFME2_REMOVE_UPGRADE_UPGRADE_MODULE_DATA_H
#define BFME2_REMOVE_UPGRADE_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"

class INI;
class MultiIniFieldParse;

class RemoveUpgradeUpgradeModuleData : public ModuleData
{
public:
	RemoveUpgradeUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	UnsignedByte m_upgradeToRemove[0xC];                   // +0x118 UpgradeToRemove; unresolved: INI::parseAsciiStringVectorAppend
	UnsignedByte m_upgradeGroupsToRemove[0xC];             // +0x124 UpgradeGroupsToRemove; unresolved: INI::parseAsciiStringVectorAppend
	Bool m_suppressEvaEventForRemoval;                     // +0x130 SuppressEvaEventForRemoval
	Bool m_removeFromAllPlayerObjects;                     // +0x131 RemoveFromAllPlayerObjects
};
// SIZE 0x134
typedef char RemoveUpgradeUpgradeModuleData_size[(sizeof(RemoveUpgradeUpgradeModuleData) == 0x134) ? 1 : -1];
typedef char RemoveUpgradeUpgradeModuleData_off_m_upgradeMuxData[(offsetof(RemoveUpgradeUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char RemoveUpgradeUpgradeModuleData_off_m_upgradeToRemove[(offsetof(RemoveUpgradeUpgradeModuleData, m_upgradeToRemove) == 0x118) ? 1 : -1];
typedef char RemoveUpgradeUpgradeModuleData_off_m_upgradeGroupsToRemove[(offsetof(RemoveUpgradeUpgradeModuleData, m_upgradeGroupsToRemove) == 0x124) ? 1 : -1];
typedef char RemoveUpgradeUpgradeModuleData_off_m_suppressEvaEventForRemoval[(offsetof(RemoveUpgradeUpgradeModuleData, m_suppressEvaEventForRemoval) == 0x130) ? 1 : -1];
typedef char RemoveUpgradeUpgradeModuleData_off_m_removeFromAllPlayerObjects[(offsetof(RemoveUpgradeUpgradeModuleData, m_removeFromAllPlayerObjects) == 0x131) ? 1 : -1];

#endif
