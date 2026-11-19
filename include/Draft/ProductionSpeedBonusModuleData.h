// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5C9A0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ProductionSpeedBonusModuleData.h: the INI data of ProductionSpeedBonus. SIZE 0x90.
// Target facts: ModuleFactory registers the module with operator new 0x90 and
// constructor RVA 0x004C2FE6; buildFieldParse RVA 0x004C2EDA calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5C9A0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_PRODUCTION_SPEED_BONUS_MODULE_DATA_H
#define BFME2_PRODUCTION_SPEED_BONUS_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char ProductionSpeedBonusModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class ProductionSpeedBonusModuleData : public SpecialPowerModuleData
{
public:
	ProductionSpeedBonusModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_numberOfFrames;                                  // +0x7C NumberOfFrames
	Real m_speedMulitplier;                                // +0x80 SpeedMulitplier
	_STL::vector<AsciiString> m_type;                      // +0x84 Type
};
// SIZE 0x90
typedef char ProductionSpeedBonusModuleData_size[(sizeof(ProductionSpeedBonusModuleData) == 0x90) ? 1 : -1];
typedef char ProductionSpeedBonusModuleData_off_m_numberOfFrames[(offsetof(ProductionSpeedBonusModuleData, m_numberOfFrames) == 0x7C) ? 1 : -1];
typedef char ProductionSpeedBonusModuleData_off_m_speedMulitplier[(offsetof(ProductionSpeedBonusModuleData, m_speedMulitplier) == 0x80) ? 1 : -1];
typedef char ProductionSpeedBonusModuleData_off_m_type[(offsetof(ProductionSpeedBonusModuleData, m_type) == 0x84) ? 1 : -1];

#endif
