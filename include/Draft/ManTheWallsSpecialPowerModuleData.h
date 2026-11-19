// DRAFT (h05 fpgen): generated from retail's field-parse table (VA none (mux tables only)).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ManTheWallsSpecialPowerModuleData.h: the INI data of SplitHordeSpecialPower. SIZE 0x7C.
// Target facts: ModuleFactory registers the module with operator new 0x7C and
// constructor RVA 0x004C72D3; buildFieldParse RVA 0x004C856F calls SpecialPowerModuleData::buildFieldParse, then adds no own table.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_MAN_THE_WALLS_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_MAN_THE_WALLS_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char ManTheWallsSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class ManTheWallsSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	ManTheWallsSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

};
// SIZE 0x7C
typedef char ManTheWallsSpecialPowerModuleData_size[(sizeof(ManTheWallsSpecialPowerModuleData) == 0x7C) ? 1 : -1];

#endif
