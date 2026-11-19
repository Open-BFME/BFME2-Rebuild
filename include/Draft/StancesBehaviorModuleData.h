// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C424B0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// StancesBehaviorModuleData.h: the INI data of StancesBehavior. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA 0x00254C8E; buildFieldParse RVA 0x0045ED8C adds VA 0x00C424B0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_STANCES_BEHAVIOR_MODULE_DATA_H
#define BFME2_STANCES_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class StancesBehaviorModuleData : public ModuleData
{
public:
	StancesBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	NameKeyType m_stanceTemplate;                          // +0x8 StanceTemplate
};
// SIZE 0xC
typedef char StancesBehaviorModuleData_size[(sizeof(StancesBehaviorModuleData) == 0xC) ? 1 : -1];
typedef char StancesBehaviorModuleData_off_m_stanceTemplate[(offsetof(StancesBehaviorModuleData, m_stanceTemplate) == 0x8) ? 1 : -1];

#endif
