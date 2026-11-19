// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C6BB18).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// HordeDispatchSpecialPowerModuleData.h: the INI data of HordeDispatchSpecialPower. SIZE 0x7C.
// Target facts: ModuleFactory registers the module with operator new 0x7C and
// constructor RVA 0x004C72D3; buildFieldParse RVA 0x004C89CB calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C6BB18 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_HORDE_DISPATCH_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_HORDE_DISPATCH_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char HordeDispatchSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class HordeDispatchSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	HordeDispatchSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

};
// SIZE 0x7C
typedef char HordeDispatchSpecialPowerModuleData_size[(sizeof(HordeDispatchSpecialPowerModuleData) == 0x7C) ? 1 : -1];

#endif
