// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C40F90).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// EnragedBehaviorModuleData.h: the INI data of EnragedBehavior. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA 0x00458F7C; buildFieldParse RVA 0x00458F92 adds VA 0x00C40F90 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ENRAGED_BEHAVIOR_MODULE_DATA_H
#define BFME2_ENRAGED_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class EnragedBehaviorModuleData : public ModuleData
{
public:
	EnragedBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_enragedLifeTimer;                               // +0x8 EnragedLifeTimer
};
// SIZE 0xC
typedef char EnragedBehaviorModuleData_size[(sizeof(EnragedBehaviorModuleData) == 0xC) ? 1 : -1];
typedef char EnragedBehaviorModuleData_off_m_enragedLifeTimer[(offsetof(EnragedBehaviorModuleData, m_enragedLifeTimer) == 0x8) ? 1 : -1];

#endif
