// InheritUpgradeCreateModuleData.h: the INI data of InheritUpgradeCreate. SIZE 0x90.
// Target facts: ModuleFactory registers the module with operator new 0x90 and
// constructor RVA 0x004B9508; buildFieldParse RVA 0x004B94B6 adds VA 0x00C59728 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_INHERIT_UPGRADE_CREATE_MODULE_DATA_H
#define BFME2_INHERIT_UPGRADE_CREATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/UpgradeMaskType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class InheritUpgradeCreateModuleData : public ModuleData
{
public:
	InheritUpgradeCreateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_radius;                                         // +0x8 Radius
	UpgradeMaskType m_upgrade;
	ObjectFilter m_objectFilter;                           // +0x8C ObjectFilter
};
// SIZE 0x90
typedef char InheritUpgradeCreateModuleData_size[(sizeof(InheritUpgradeCreateModuleData) == 0x90) ? 1 : -1];
typedef char InheritUpgradeCreateModuleData_off_m_radius[(offsetof(InheritUpgradeCreateModuleData, m_radius) == 0x8) ? 1 : -1];
typedef char InheritUpgradeCreateModuleData_off_m_upgrade[(offsetof(InheritUpgradeCreateModuleData, m_upgrade) == 0xC) ? 1 : -1];
typedef char InheritUpgradeCreateModuleData_off_m_objectFilter[(offsetof(InheritUpgradeCreateModuleData, m_objectFilter) == 0x8C) ? 1 : -1];

#endif
