// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C579B4).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// StatusBitsUpgradeIfEldestKindofModuleData.h: the INI data of StatusBitsUpgradeIfEldestKindof. SIZE 0x13C.
// Target facts: ModuleFactory registers the module with operator new 0x13C and
// constructor RVA 0x002547D1; buildFieldParse RVA 0x004B4A17 calls StatusBitsUpgradeModuleData::buildFieldParse, then adds VA 0x00C579B4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_STATUS_BITS_UPGRADE_IF_ELDEST_KINDOF_MODULE_DATA_H
#define BFME2_STATUS_BITS_UPGRADE_IF_ELDEST_KINDOF_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/StatusBitsUpgradeModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

// Base StatusBitsUpgradeModuleData: the constructor calls its constructor on this. It ends at 0x138 (its own operator-new size);
// this class's members follow it.
typedef char StatusBitsUpgradeIfEldestKindofModuleData_base_size[(sizeof(StatusBitsUpgradeModuleData) == 0x138) ? 1 : -1];

class StatusBitsUpgradeIfEldestKindofModuleData : public StatusBitsUpgradeModuleData
{
public:
	StatusBitsUpgradeIfEldestKindofModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	ObjectFilter m_objectFilter;                           // +0x138 ObjectFilter
};
// SIZE 0x13C
typedef char StatusBitsUpgradeIfEldestKindofModuleData_size[(sizeof(StatusBitsUpgradeIfEldestKindofModuleData) == 0x13C) ? 1 : -1];
typedef char StatusBitsUpgradeIfEldestKindofModuleData_off_m_objectFilter[(offsetof(StatusBitsUpgradeIfEldestKindofModuleData, m_objectFilter) == 0x138) ? 1 : -1];

#endif
