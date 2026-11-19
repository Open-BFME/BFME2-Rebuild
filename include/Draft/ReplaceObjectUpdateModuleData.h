// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BEF960).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ReplaceObjectUpdateModuleData.h: the INI data of ReplaceObjectUpdate. SIZE 0xE0.
// Target facts: ModuleFactory registers the module with operator new 0xE0 and
// constructor RVA 0x004B2AC4; buildFieldParse RVA 0x0024FC94 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEF960 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_REPLACE_OBJECT_UPDATE_MODULE_DATA_H
#define BFME2_REPLACE_OBJECT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char ReplaceObjectUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class ReplaceObjectUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	ReplaceObjectUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_unk0xC8[0xC];
	Real m_replaceRadius;                                  // +0xD4 ReplaceRadius
	const FXList * m_replaceFX;                            // +0xD8 ReplaceFX
	Bool m_scatter;                                        // +0xDC Scatter
};
// SIZE 0xE0
typedef char ReplaceObjectUpdateModuleData_size[(sizeof(ReplaceObjectUpdateModuleData) == 0xE0) ? 1 : -1];
typedef char ReplaceObjectUpdateModuleData_off_m_replaceRadius[(offsetof(ReplaceObjectUpdateModuleData, m_replaceRadius) == 0xD4) ? 1 : -1];
typedef char ReplaceObjectUpdateModuleData_off_m_replaceFX[(offsetof(ReplaceObjectUpdateModuleData, m_replaceFX) == 0xD8) ? 1 : -1];
typedef char ReplaceObjectUpdateModuleData_off_m_scatter[(offsetof(ReplaceObjectUpdateModuleData, m_scatter) == 0xDC) ? 1 : -1];

#endif
