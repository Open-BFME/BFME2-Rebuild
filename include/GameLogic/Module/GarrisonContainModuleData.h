// GarrisonContainModuleData.h: the INI data of GarrisonContain. SIZE 0xAC.
// Target facts: ModuleFactory registers the module with operator new 0xAC and
// constructor RVA 0x0047978F; buildFieldParse RVA 0x00254F75 calls OpenContainModuleData::buildFieldParse, then adds VA 0x00BF3360 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_GARRISON_CONTAIN_MODULE_DATA_H
#define BFME2_GARRISON_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/OpenContainModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base OpenContainModuleData: the constructor calls its constructor on this. It ends at 0x98 (its own operator-new size);
// this class's members follow it.
typedef char GarrisonContainModuleData_base_size[(sizeof(OpenContainModuleData) == 0x98) ? 1 : -1];

class GarrisonContainModuleData : public OpenContainModuleData
{
public:
	GarrisonContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);
	static void parseInitialRoster(INI *ini, void *instance, void *store, const void *userData);

	Bool m_healObjects;                                    // +0x98 HealObjects
	Real m_timeForFullHeal;                                // +0x9C TimeForFullHeal; duration
	Bool m_mobileGarrison;                                 // +0xA0 MobileGarrison
	Bool m_immuneToClearBuildingAttacks;                   // +0xA1 ImmuneToClearBuildingAttacks
	UnsignedByte m_unk0xA2[2];
	AsciiString m_initialRosterTemplateName;               // +0xA4 InitialRoster's template name; the destructor 0x00257507 releases it, onObjectCreated 0x004787B0 looks it up
	Int m_initialRosterCount;                              // +0xA8 InitialRoster's count (the constructor 0x0047978F zeroes it)
};
// SIZE 0xAC
typedef char GarrisonContainModuleData_size[(sizeof(GarrisonContainModuleData) == 0xAC) ? 1 : -1];
typedef char GarrisonContainModuleData_off_m_healObjects[(offsetof(GarrisonContainModuleData, m_healObjects) == 0x98) ? 1 : -1];
typedef char GarrisonContainModuleData_off_m_timeForFullHeal[(offsetof(GarrisonContainModuleData, m_timeForFullHeal) == 0x9C) ? 1 : -1];
typedef char GarrisonContainModuleData_off_m_mobileGarrison[(offsetof(GarrisonContainModuleData, m_mobileGarrison) == 0xA0) ? 1 : -1];
typedef char GarrisonContainModuleData_off_m_initialRosterTemplateName[(offsetof(GarrisonContainModuleData, m_initialRosterTemplateName) == 0xA4) ? 1 : -1];
typedef char GarrisonContainModuleData_off_m_initialRosterCount[(offsetof(GarrisonContainModuleData, m_initialRosterCount) == 0xA8) ? 1 : -1];
typedef char GarrisonContainModuleData_off_m_immuneToClearBuildingAttacks[(offsetof(GarrisonContainModuleData, m_immuneToClearBuildingAttacks) == 0xA1) ? 1 : -1];

#endif
