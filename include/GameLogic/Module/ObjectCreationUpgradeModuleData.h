// Promoted from Draft by f03: m_deathAnimAndDuration is the CustomAnimAndDuration the parse function
// stores (constructor 0x004B425B writes -1, 0, 0 there; ObjectCreationUpgrade 0x004B463C reads its
// animation type at +0x140 and its frame count at +0x144).
// ObjectCreationUpgradeModuleData.h: the INI data of ObjectCreationUpgrade. SIZE 0x154.
// Target facts: ModuleFactory registers the module with operator new 0x154 and
// constructor RVA 0x004B425B; buildFieldParse RVA 0x004B4084 adds VA 0x00C57708 at +0x0 and UpgradeMuxData at +0x10.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_OBJECT_CREATION_UPGRADE_MODULE_DATA_H
#define BFME2_OBJECT_CREATION_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/CustomAnimAndDuration.h"

class INI;
class MultiIniFieldParse;
class ObjectCreationList;

class ObjectCreationUpgradeModuleData : public ModuleData
{
public:
	ObjectCreationUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const ObjectCreationList * m_upgradeObject;            // +0x8 UpgradeObject
	UnsignedInt m_delay;                                   // +0xC Delay; duration, frames
	UpgradeMuxData m_upgradeMuxData;                       // +0x10; added by buildFieldParse at +0x10
	AsciiString m_removeUpgrade;                           // +0x120 RemoveUpgrade
	AsciiString m_grantUpgrade;                            // +0x124 GrantUpgrade
	AsciiString m_thingToSpawn;                            // +0x128 ThingToSpawn
	Coord3D m_offset;                                      // +0x12C Offset
	Real m_angle;                                          // +0x138 Angle; angle
	Bool m_destroyWhenSold;                                // +0x13C DestroyWhenSold
	UnsignedByte m_unk0x13D[0x3];
	CustomAnimAndDuration m_deathAnimAndDuration;          // +0x140 DeathAnimAndDuration; INI::iniParseAnimAndDuration (0x0044E6CD); constructor 0x004B425B stores -1, 0, 0 here
	UnsignedInt m_fadeInTime;                              // +0x14C FadeInTime
	Bool m_useBuildingProduction;                          // +0x150 UseBuildingProduction
};
// SIZE 0x154
typedef char ObjectCreationUpgradeModuleData_size[(sizeof(ObjectCreationUpgradeModuleData) == 0x154) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_upgradeObject[(offsetof(ObjectCreationUpgradeModuleData, m_upgradeObject) == 0x8) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_delay[(offsetof(ObjectCreationUpgradeModuleData, m_delay) == 0xC) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_upgradeMuxData[(offsetof(ObjectCreationUpgradeModuleData, m_upgradeMuxData) == 0x10) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_removeUpgrade[(offsetof(ObjectCreationUpgradeModuleData, m_removeUpgrade) == 0x120) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_grantUpgrade[(offsetof(ObjectCreationUpgradeModuleData, m_grantUpgrade) == 0x124) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_thingToSpawn[(offsetof(ObjectCreationUpgradeModuleData, m_thingToSpawn) == 0x128) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_offset[(offsetof(ObjectCreationUpgradeModuleData, m_offset) == 0x12C) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_angle[(offsetof(ObjectCreationUpgradeModuleData, m_angle) == 0x138) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_destroyWhenSold[(offsetof(ObjectCreationUpgradeModuleData, m_destroyWhenSold) == 0x13C) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_deathAnimAndDuration[(offsetof(ObjectCreationUpgradeModuleData, m_deathAnimAndDuration) == 0x140) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_fadeInTime[(offsetof(ObjectCreationUpgradeModuleData, m_fadeInTime) == 0x14C) ? 1 : -1];
typedef char ObjectCreationUpgradeModuleData_off_m_useBuildingProduction[(offsetof(ObjectCreationUpgradeModuleData, m_useBuildingProduction) == 0x150) ? 1 : -1];

#endif
