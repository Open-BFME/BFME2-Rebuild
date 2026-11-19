// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C4C2A8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// OilSpillUpdateModuleData.h: the INI data of OilSpillUpdate. SIZE 0x20.
// Target facts: ModuleFactory registers the module with operator new 0x20 and
// constructor RVA 0x0048C16C; buildFieldParse RVA 0x0048C151 calls FireWeaponUpdateModuleData::buildFieldParse, then adds VA 0x00C4C2A8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_OIL_SPILL_UPDATE_MODULE_DATA_H
#define BFME2_OIL_SPILL_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/FireWeaponUpdate.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base FireWeaponUpdateModuleData: the constructor calls its constructor on this. It ends at 0x10 (its own operator-new size);
// this class's members follow it.
typedef char OilSpillUpdateModuleData_base_size[(sizeof(FireWeaponUpdateModuleData) == 0x10) ? 1 : -1];

class OilSpillUpdateModuleData : public FireWeaponUpdateModuleData
{
public:
	OilSpillUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_breadcrumbName;                          // +0x10 BreadcrumbName
	AsciiString m_ignitionWeaponName;                      // +0x14 IgnitionWeaponName
	Real m_ignitionWeaponSpacing;                          // +0x18 IgnitionWeaponSpacing
	const FXList * m_oilSpillFX;                           // +0x1C OilSpillFX
};
// SIZE 0x20
typedef char OilSpillUpdateModuleData_size[(sizeof(OilSpillUpdateModuleData) == 0x20) ? 1 : -1];
typedef char OilSpillUpdateModuleData_off_m_breadcrumbName[(offsetof(OilSpillUpdateModuleData, m_breadcrumbName) == 0x10) ? 1 : -1];
typedef char OilSpillUpdateModuleData_off_m_ignitionWeaponName[(offsetof(OilSpillUpdateModuleData, m_ignitionWeaponName) == 0x14) ? 1 : -1];
typedef char OilSpillUpdateModuleData_off_m_ignitionWeaponSpacing[(offsetof(OilSpillUpdateModuleData, m_ignitionWeaponSpacing) == 0x18) ? 1 : -1];
typedef char OilSpillUpdateModuleData_off_m_oilSpillFX[(offsetof(OilSpillUpdateModuleData, m_oilSpillFX) == 0x1C) ? 1 : -1];

#endif
