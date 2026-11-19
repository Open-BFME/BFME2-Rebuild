// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C6BB18).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// SiegeAIUpdateModuleData.h: the INI data of SiegeAIUpdate. SIZE 0x64.
// Target facts: ModuleFactory registers the module with operator new 0x64 and
// constructor RVA 0x004904D9; buildFieldParse RVA 0x004904EB calls AIUpdateModuleData::buildFieldParse, then adds VA 0x00C6BB18 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SIEGE_A_I_UPDATE_MODULE_DATA_H
#define BFME2_SIEGE_A_I_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class INI;
class MultiIniFieldParse;

// Base AIUpdateModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char SiegeAIUpdateModuleData_base_size[(sizeof(AIUpdateModuleData) == 0x64) ? 1 : -1];

class SiegeAIUpdateModuleData : public AIUpdateModuleData
{
public:
	SiegeAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

};
// SIZE 0x64
typedef char SiegeAIUpdateModuleData_size[(sizeof(SiegeAIUpdateModuleData) == 0x64) ? 1 : -1];

#endif
