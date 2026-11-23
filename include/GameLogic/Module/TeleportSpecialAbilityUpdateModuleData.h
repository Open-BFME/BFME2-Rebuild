// TeleportSpecialAbilityUpdateModuleData.h: the INI data of TeleportSpecialAbilityUpdate. SIZE 0xD8.
// Target facts: ModuleFactory registers the module with operator new 0xD8 and
// constructor RVA 0x00492E43; buildFieldParse RVA 0x0024DBAA calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEF150 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_TELEPORT_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_TELEPORT_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char TeleportSpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class TeleportSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	TeleportSpecialAbilityUpdateModuleData();
	virtual ~TeleportSpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_busyForDuration;                         // +0xC8 BusyForDuration; duration, frames
	AsciiString m_destinationWeaponName;                   // +0xCC DestinationWeaponName
	AsciiString m_sourceWeaponName;                        // +0xD0 SourceWeaponName
	Real m_maxDistance;                                    // +0xD4 MaxDistance
};
// SIZE 0xD8
typedef char TeleportSpecialAbilityUpdateModuleData_size[(sizeof(TeleportSpecialAbilityUpdateModuleData) == 0xD8) ? 1 : -1];
typedef char TeleportSpecialAbilityUpdateModuleData_off_m_busyForDuration[(offsetof(TeleportSpecialAbilityUpdateModuleData, m_busyForDuration) == 0xC8) ? 1 : -1];
typedef char TeleportSpecialAbilityUpdateModuleData_off_m_destinationWeaponName[(offsetof(TeleportSpecialAbilityUpdateModuleData, m_destinationWeaponName) == 0xCC) ? 1 : -1];
typedef char TeleportSpecialAbilityUpdateModuleData_off_m_sourceWeaponName[(offsetof(TeleportSpecialAbilityUpdateModuleData, m_sourceWeaponName) == 0xD0) ? 1 : -1];
typedef char TeleportSpecialAbilityUpdateModuleData_off_m_maxDistance[(offsetof(TeleportSpecialAbilityUpdateModuleData, m_maxDistance) == 0xD4) ? 1 : -1];

#endif
