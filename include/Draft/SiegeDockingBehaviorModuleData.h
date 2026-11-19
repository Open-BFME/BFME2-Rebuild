// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C413E4).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// SiegeDockingBehaviorModuleData.h: the INI data of SiegeDockingBehavior. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA 0x00254C8E; buildFieldParse RVA 0x004598E1 adds VA 0x00C413E4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SIEGE_DOCKING_BEHAVIOR_MODULE_DATA_H
#define BFME2_SIEGE_DOCKING_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class SiegeDockingBehaviorModuleData : public ModuleData
{
public:
	SiegeDockingBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_dummy;                                           // +0x8 DUMMY
};
// SIZE 0xC
typedef char SiegeDockingBehaviorModuleData_size[(sizeof(SiegeDockingBehaviorModuleData) == 0xC) ? 1 : -1];
typedef char SiegeDockingBehaviorModuleData_off_m_dummy[(offsetof(SiegeDockingBehaviorModuleData, m_dummy) == 0x8) ? 1 : -1];

#endif
