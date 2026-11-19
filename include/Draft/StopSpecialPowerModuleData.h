// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5DDE4).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// StopSpecialPowerModuleData.h: the INI data of StopSpecialPower. SIZE 0x80.
// Target facts: ModuleFactory registers the module with operator new 0x80 and
// constructor RVA 0x004C68AB; buildFieldParse RVA 0x004C6751 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5DDE4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_STOP_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_STOP_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class INI;
class MultiIniFieldParse;
class SpecialPowerTemplate;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char StopSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class StopSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	StopSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const SpecialPowerTemplate * m_stopPowerTemplate;      // +0x7C StopPowerTemplate
};
// SIZE 0x80
typedef char StopSpecialPowerModuleData_size[(sizeof(StopSpecialPowerModuleData) == 0x80) ? 1 : -1];
typedef char StopSpecialPowerModuleData_off_m_stopPowerTemplate[(offsetof(StopSpecialPowerModuleData, m_stopPowerTemplate) == 0x7C) ? 1 : -1];

#endif
