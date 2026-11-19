// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C6BB18).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ShipSlowDeathBehaviorModuleData.h: the INI data of ShipSlowDeathBehavior. SIZE 0x190.
// Target facts: ModuleFactory registers the module with operator new 0x190 and
// constructor RVA 0x0045E961; buildFieldParse RVA 0x0045E973 calls SlowDeathBehaviorModuleData::buildFieldParse, then adds VA 0x00C6BB18 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SHIP_SLOW_DEATH_BEHAVIOR_MODULE_DATA_H
#define BFME2_SHIP_SLOW_DEATH_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/SlowDeathBehaviorModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SlowDeathBehaviorModuleData: the constructor calls its constructor on this. It ends at 0x190 (its own operator-new size);
// this class's members follow it.
typedef char ShipSlowDeathBehaviorModuleData_base_size[(sizeof(SlowDeathBehaviorModuleData) == 0x190) ? 1 : -1];

class ShipSlowDeathBehaviorModuleData : public SlowDeathBehaviorModuleData
{
public:
	ShipSlowDeathBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

};
// SIZE 0x190
typedef char ShipSlowDeathBehaviorModuleData_size[(sizeof(ShipSlowDeathBehaviorModuleData) == 0x190) ? 1 : -1];

#endif
