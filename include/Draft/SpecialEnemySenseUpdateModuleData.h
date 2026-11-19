// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF1B88).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// SpecialEnemySenseUpdateModuleData.h: the INI data of SpecialEnemySenseUpdate. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x0025404A; buildFieldParse RVA 0x0025408B adds VA 0x00BF1B88 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SPECIAL_ENEMY_SENSE_UPDATE_MODULE_DATA_H
#define BFME2_SPECIAL_ENEMY_SENSE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class SpecialEnemySenseUpdateModuleData : public ModuleData
{
public:
	SpecialEnemySenseUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	ObjectFilter m_specialEnemyFilter;                     // +0x8 SpecialEnemyFilter
	Real m_scanRange;                                      // +0xC ScanRange
	UnsignedInt m_scanInterval;                            // +0x10 ScanInterval; duration, frames
};
// SIZE 0x14
typedef char SpecialEnemySenseUpdateModuleData_size[(sizeof(SpecialEnemySenseUpdateModuleData) == 0x14) ? 1 : -1];
typedef char SpecialEnemySenseUpdateModuleData_off_m_specialEnemyFilter[(offsetof(SpecialEnemySenseUpdateModuleData, m_specialEnemyFilter) == 0x8) ? 1 : -1];
typedef char SpecialEnemySenseUpdateModuleData_off_m_scanRange[(offsetof(SpecialEnemySenseUpdateModuleData, m_scanRange) == 0xC) ? 1 : -1];
typedef char SpecialEnemySenseUpdateModuleData_off_m_scanInterval[(offsetof(SpecialEnemySenseUpdateModuleData, m_scanInterval) == 0x10) ? 1 : -1];

#endif
