// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF0994).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// KeepObjectDieModuleData.h: the INI data of KeepObjectDie. SIZE 0x40.
// Target facts: ModuleFactory registers the module with operator new 0x40 and
// constructor RVA 0x00253B78; buildFieldParse RVA 0x00253B95 adds VA 0x00BF0994 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_KEEP_OBJECT_DIE_MODULE_DATA_H
#define BFME2_KEEP_OBJECT_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"

class INI;
class MultiIniFieldParse;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char KeepObjectDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class KeepObjectDieModuleData : public DestroyDieModuleData
{
public:
	KeepObjectDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_collapsingTime;                          // +0x38 CollapsingTime; duration, frames
	Bool m_stayOnRadar;                                    // +0x3C StayOnRadar
};
// SIZE 0x40
typedef char KeepObjectDieModuleData_size[(sizeof(KeepObjectDieModuleData) == 0x40) ? 1 : -1];
typedef char KeepObjectDieModuleData_off_m_collapsingTime[(offsetof(KeepObjectDieModuleData, m_collapsingTime) == 0x38) ? 1 : -1];
typedef char KeepObjectDieModuleData_off_m_stayOnRadar[(offsetof(KeepObjectDieModuleData, m_stayOnRadar) == 0x3C) ? 1 : -1];

#endif
