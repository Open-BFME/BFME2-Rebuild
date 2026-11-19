// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5D6C0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// FreezingRainSpecialPowerModuleData.h: the INI data of FreezingRainSpecialPower. SIZE 0x88.
// Target facts: ModuleFactory registers the module with operator new 0x88 and
// constructor RVA 0x004C4D71; buildFieldParse RVA 0x004C4B86 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5D6C0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_FREEZING_RAIN_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_FREEZING_RAIN_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char FreezingRainSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class FreezingRainSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	FreezingRainSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_freezingRainRadius;                             // +0x7C FreezingRainRadius
	const FXList * m_freezingRainFX;                       // +0x80 FreezingRainFX
	Int m_burnRateModifier;                                // +0x84 BurnRateModifier
};
// SIZE 0x88
typedef char FreezingRainSpecialPowerModuleData_size[(sizeof(FreezingRainSpecialPowerModuleData) == 0x88) ? 1 : -1];
typedef char FreezingRainSpecialPowerModuleData_off_m_freezingRainRadius[(offsetof(FreezingRainSpecialPowerModuleData, m_freezingRainRadius) == 0x7C) ? 1 : -1];
typedef char FreezingRainSpecialPowerModuleData_off_m_freezingRainFX[(offsetof(FreezingRainSpecialPowerModuleData, m_freezingRainFX) == 0x80) ? 1 : -1];
typedef char FreezingRainSpecialPowerModuleData_off_m_burnRateModifier[(offsetof(FreezingRainSpecialPowerModuleData, m_burnRateModifier) == 0x84) ? 1 : -1];

#endif
