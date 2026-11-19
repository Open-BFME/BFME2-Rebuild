// CreateObjectDieIfEldestKindofModuleData.h: the INI data of CreateObjectDieIfEldestKindof. SIZE 0x50.
// Target facts: ModuleFactory registers the module with operator new 0x50 and
// constructor RVA 0x00485E19; buildFieldParse RVA 0x00485B30 calls CreateObjectDieModuleData::buildFieldParse, then adds VA 0x00C4A968 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CREATE_OBJECT_DIE_IF_ELDEST_KINDOF_MODULE_DATA_H
#define BFME2_CREATE_OBJECT_DIE_IF_ELDEST_KINDOF_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CreateObjectDieModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

// Base CreateObjectDieModuleData: the constructor calls its constructor on this. It ends at 0x4C (its own operator-new size);
// this class's members follow it.
typedef char CreateObjectDieIfEldestKindofModuleData_base_size[(sizeof(CreateObjectDieModuleData) == 0x4C) ? 1 : -1];

class CreateObjectDieIfEldestKindofModuleData : public CreateObjectDieModuleData
{
public:
	CreateObjectDieIfEldestKindofModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	ObjectFilter m_objectFilter;                           // +0x4C ObjectFilter
};
// SIZE 0x50
typedef char CreateObjectDieIfEldestKindofModuleData_size[(sizeof(CreateObjectDieIfEldestKindofModuleData) == 0x50) ? 1 : -1];
typedef char CreateObjectDieIfEldestKindofModuleData_off_m_objectFilter[(offsetof(CreateObjectDieIfEldestKindofModuleData, m_objectFilter) == 0x4C) ? 1 : -1];

#endif
