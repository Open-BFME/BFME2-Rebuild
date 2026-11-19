// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C1B00C).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// SpawnedModelConditionCreateModuleData.h: the INI data of SpawnedModelConditionCreate. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x00257840; buildFieldParse RVA 0x003A49C0 adds VA 0x00C1B00C at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SPAWNED_MODEL_CONDITION_CREATE_MODULE_DATA_H
#define BFME2_SPAWNED_MODEL_CONDITION_CREATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class SpawnedModelConditionCreateModuleData : public ModuleData
{
public:
	SpawnedModelConditionCreateModuleData() throw();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedByte m_unk0x8[0xC];
};
// SIZE 0x14
typedef char SpawnedModelConditionCreateModuleData_size[(sizeof(SpawnedModelConditionCreateModuleData) == 0x14) ? 1 : -1];

#endif
