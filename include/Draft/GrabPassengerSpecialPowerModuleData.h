// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5D8A8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// GrabPassengerSpecialPowerModuleData.h: the INI data of GrabPassengerSpecialPower. SIZE 0x84.
// Target facts: ModuleFactory registers the module with operator new 0x84 and
// constructor RVA 0x004C5421; buildFieldParse RVA 0x004C4F89 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5D8A8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_GRAB_PASSENGER_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_GRAB_PASSENGER_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char GrabPassengerSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class GrabPassengerSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	GrabPassengerSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_grabRadius;                                     // +0x7C GrabRadius
	Bool m_allowTree;                                      // +0x80 AllowTree
};
// SIZE 0x84
typedef char GrabPassengerSpecialPowerModuleData_size[(sizeof(GrabPassengerSpecialPowerModuleData) == 0x84) ? 1 : -1];
typedef char GrabPassengerSpecialPowerModuleData_off_m_grabRadius[(offsetof(GrabPassengerSpecialPowerModuleData, m_grabRadius) == 0x7C) ? 1 : -1];
typedef char GrabPassengerSpecialPowerModuleData_off_m_allowTree[(offsetof(GrabPassengerSpecialPowerModuleData, m_allowTree) == 0x80) ? 1 : -1];

#endif
