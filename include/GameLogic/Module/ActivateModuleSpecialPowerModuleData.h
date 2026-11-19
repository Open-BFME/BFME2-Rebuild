// ActivateModuleSpecialPowerModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x00256DC5);
// name, size, bases and virtual order are retail facts given below.
// ActivateModuleSpecialPowerModuleData.h: the INI data of ActivateModuleSpecialPower. SIZE 0xD4.
// Target facts: ModuleFactory registers the module with operator new 0xD4 and
// constructor RVA 0x00256DA1; buildFieldParse RVA 0x004CDFBE calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C5FD70 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ACTIVATE_MODULE_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_ACTIVATE_MODULE_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include <vector>

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char ActivateModuleSpecialPowerModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

// One entry of the +0xC8 wake list: the module key SetWakeState (0x004CDDBA) looks up on the object, and a mode
// (1 reads the position from the object, anything else from the special ability's target).
struct ActivateModuleWakeEntry
{
	Int m_key;
	Int m_mode;
};

class ActivateModuleSpecialPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	ActivateModuleSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x00256DC5

	std::vector<ActivateModuleWakeEntry> m_wakeList;       // +0xC8 (8-byte {key, mode} entries; SetWakeState 0x004CDDBA walks begin..end)
};
// SIZE 0xD4
typedef char ActivateModuleSpecialPowerModuleData_size[(sizeof(ActivateModuleSpecialPowerModuleData) == 0xD4) ? 1 : -1];

#endif
