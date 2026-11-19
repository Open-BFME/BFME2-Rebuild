// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF2398).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DefaultProductionExitUpdateModuleData.h: the INI data of SupplyCenterProductionExitUpdate. SIZE 0x20.
// Target facts: ModuleFactory registers the module with operator new 0x20 and
// constructor RVA ?; buildFieldParse RVA 0x00253CCC adds VA 0x00BF2398 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DEFAULT_PRODUCTION_EXIT_UPDATE_MODULE_DATA_H
#define BFME2_DEFAULT_PRODUCTION_EXIT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/Coord3D.h"

class INI;
class MultiIniFieldParse;

class DefaultProductionExitUpdateModuleData : public ModuleData
{
public:
	DefaultProductionExitUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Coord3D m_unitCreatePoint;                             // +0x8 UnitCreatePoint
	Coord3D m_naturalRallyPoint;                           // +0x14 NaturalRallyPoint
};
// SIZE 0x20
typedef char DefaultProductionExitUpdateModuleData_size[(sizeof(DefaultProductionExitUpdateModuleData) == 0x20) ? 1 : -1];
typedef char DefaultProductionExitUpdateModuleData_off_m_unitCreatePoint[(offsetof(DefaultProductionExitUpdateModuleData, m_unitCreatePoint) == 0x8) ? 1 : -1];
typedef char DefaultProductionExitUpdateModuleData_off_m_naturalRallyPoint[(offsetof(DefaultProductionExitUpdateModuleData, m_naturalRallyPoint) == 0x14) ? 1 : -1];

#endif
