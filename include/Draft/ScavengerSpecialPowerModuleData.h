// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5D27C).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ScavengerSpecialPowerModuleData.h: the INI data of ScavengerSpecialPower. SIZE 0x80.
// Target facts: ModuleFactory registers the module with operator new 0x80 and
// constructor RVA 0x004C2A6A; buildFieldParse RVA 0x004C4338 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5D27C at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SCAVENGER_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_SCAVENGER_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char ScavengerSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class ScavengerSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	ScavengerSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_bountyPercent;                                  // +0x7C BountyPercent
};
// SIZE 0x80
typedef char ScavengerSpecialPowerModuleData_size[(sizeof(ScavengerSpecialPowerModuleData) == 0x80) ? 1 : -1];
typedef char ScavengerSpecialPowerModuleData_off_m_bountyPercent[(offsetof(ScavengerSpecialPowerModuleData, m_bountyPercent) == 0x7C) ? 1 : -1];

#endif
