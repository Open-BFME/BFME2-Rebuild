// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5B8BC).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// RespawnBodyModuleData.h: the INI data of RespawnBody. SIZE 0x6C.
// Target facts: ModuleFactory registers the module with operator new 0x6C and
// constructor RVA 0x004C14DF; buildFieldParse RVA 0x004C12D0 calls ActiveBodyModuleData::buildFieldParse, then adds VA 0x00C5B8BC at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_RESPAWN_BODY_MODULE_DATA_H
#define BFME2_RESPAWN_BODY_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBodyModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

// Base ActiveBodyModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char RespawnBodyModuleData_base_size[(sizeof(ActiveBodyModuleData) == 0x64) ? 1 : -1];

class RespawnBodyModuleData : public ActiveBodyModuleData
{
public:
	RespawnBodyModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	ObjectFilter m_permanentlyKilledByFilter;              // +0x64 PermanentlyKilledByFilter
	Bool m_canRespawn;                                     // +0x68 CanRespawn
};
// SIZE 0x6C
typedef char RespawnBodyModuleData_size[(sizeof(RespawnBodyModuleData) == 0x6C) ? 1 : -1];
typedef char RespawnBodyModuleData_off_m_permanentlyKilledByFilter[(offsetof(RespawnBodyModuleData, m_permanentlyKilledByFilter) == 0x64) ? 1 : -1];
typedef char RespawnBodyModuleData_off_m_canRespawn[(offsetof(RespawnBodyModuleData, m_canRespawn) == 0x68) ? 1 : -1];

#endif
