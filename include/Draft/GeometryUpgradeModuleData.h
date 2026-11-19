// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C58918).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_showGeometry, m_hideGeometry.
// GeometryUpgradeModuleData.h: the INI data of GeometryUpgrade. SIZE 0x13C.
// Target facts: ModuleFactory registers the module with operator new 0x13C and
// constructor RVA 0x004B6EE2; buildFieldParse RVA 0x004B6A3C adds VA 0x00C58918 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_showGeometry +0x118: INI::parseAsciiStringVectorAppend 0x0002E896
//   m_hideGeometry +0x124: INI::parseAsciiStringVectorAppend 0x0002E896
#pragma once
#ifndef BFME2_GEOMETRY_UPGRADE_MODULE_DATA_H
#define BFME2_GEOMETRY_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class GeometryUpgradeModuleData : public ModuleData
{
public:
	GeometryUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	UnsignedByte m_showGeometry[0xC];                      // +0x118 ShowGeometry; unresolved: INI::parseAsciiStringVectorAppend
	UnsignedByte m_hideGeometry[0xC];                      // +0x124 HideGeometry; unresolved: INI::parseAsciiStringVectorAppend
	AsciiString m_wallBoundsMesh;                          // +0x130 WallBoundsMesh
	AsciiString m_rampMesh1;                               // +0x134 RampMesh1
	AsciiString m_rampMesh2;                               // +0x138 RampMesh2
};
// SIZE 0x13C
typedef char GeometryUpgradeModuleData_size[(sizeof(GeometryUpgradeModuleData) == 0x13C) ? 1 : -1];
typedef char GeometryUpgradeModuleData_off_m_upgradeMuxData[(offsetof(GeometryUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char GeometryUpgradeModuleData_off_m_showGeometry[(offsetof(GeometryUpgradeModuleData, m_showGeometry) == 0x118) ? 1 : -1];
typedef char GeometryUpgradeModuleData_off_m_hideGeometry[(offsetof(GeometryUpgradeModuleData, m_hideGeometry) == 0x124) ? 1 : -1];
typedef char GeometryUpgradeModuleData_off_m_wallBoundsMesh[(offsetof(GeometryUpgradeModuleData, m_wallBoundsMesh) == 0x130) ? 1 : -1];
typedef char GeometryUpgradeModuleData_off_m_rampMesh1[(offsetof(GeometryUpgradeModuleData, m_rampMesh1) == 0x134) ? 1 : -1];
typedef char GeometryUpgradeModuleData_off_m_rampMesh2[(offsetof(GeometryUpgradeModuleData, m_rampMesh2) == 0x138) ? 1 : -1];

#endif
