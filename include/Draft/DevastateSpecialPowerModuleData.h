// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5E3D0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DevastateSpecialPowerModuleData.h: the INI data of DevastateSpecialPower. SIZE 0x90.
// Target facts: ModuleFactory registers the module with operator new 0x90 and
// constructor RVA 0x004C84BD; buildFieldParse RVA 0x004C81C8 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5E3D0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DEVASTATE_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_DEVASTATE_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char DevastateSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class DevastateSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	DevastateSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_radius;                                         // +0x7C Radius
	const FXList * m_fx;                                   // +0x80 FX
	Real m_treeValueMultiplier;                            // +0x84 TreeValueMultiplier; percent
	Real m_treeValueTotalCap;                              // +0x88 TreeValueTotalCap
	AsciiString m_fireWeapon;                              // +0x8C FireWeapon
};
// SIZE 0x90
typedef char DevastateSpecialPowerModuleData_size[(sizeof(DevastateSpecialPowerModuleData) == 0x90) ? 1 : -1];
typedef char DevastateSpecialPowerModuleData_off_m_radius[(offsetof(DevastateSpecialPowerModuleData, m_radius) == 0x7C) ? 1 : -1];
typedef char DevastateSpecialPowerModuleData_off_m_fx[(offsetof(DevastateSpecialPowerModuleData, m_fx) == 0x80) ? 1 : -1];
typedef char DevastateSpecialPowerModuleData_off_m_treeValueMultiplier[(offsetof(DevastateSpecialPowerModuleData, m_treeValueMultiplier) == 0x84) ? 1 : -1];
typedef char DevastateSpecialPowerModuleData_off_m_treeValueTotalCap[(offsetof(DevastateSpecialPowerModuleData, m_treeValueTotalCap) == 0x88) ? 1 : -1];
typedef char DevastateSpecialPowerModuleData_off_m_fireWeapon[(offsetof(DevastateSpecialPowerModuleData, m_fireWeapon) == 0x8C) ? 1 : -1];

#endif
