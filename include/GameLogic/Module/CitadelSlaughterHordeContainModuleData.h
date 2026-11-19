// CitadelSlaughterHordeContainModuleData.h: the INI data of CitadelSlaughterHordeContain. SIZE 0x110.
// Target facts: ModuleFactory registers the module with operator new 0x110 and
// constructor RVA 0x0048112F; buildFieldParse RVA 0x004802F2 calls SlaughterHordeContainModuleData::buildFieldParse, then adds VA 0x00C48890 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CITADEL_SLAUGHTER_HORDE_CONTAIN_MODULE_DATA_H
#define BFME2_CITADEL_SLAUGHTER_HORDE_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SlaughterHordeContainModuleData.h"
#include "Common/BitFlags.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SlaughterHordeContainModuleData: the constructor calls its constructor on this. It ends at 0xEC (its own operator-new size);
// this class's members follow it.
typedef char CitadelSlaughterHordeContainModuleData_base_size[(sizeof(SlaughterHordeContainModuleData) == 0xEC) ? 1 : -1];

class CitadelSlaughterHordeContainModuleData : public SlaughterHordeContainModuleData
{
public:
	CitadelSlaughterHordeContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	ObjectStatusMaskType m_statusForRingEntry;             // +0xEC StatusForRingEntry; BitFlags<101>
	ObjectFilter m_objectToDestroyForRingEntry;            // +0xFC ObjectToDestroyForRingEntry
	_STL::vector<AsciiString> m_upgradeForRingEntry;       // +0x100 UpgradeForRingEntry
	const FXList * m_fxForRingEntry;                       // +0x10C FXForRingEntry
};
// SIZE 0x110
typedef char CitadelSlaughterHordeContainModuleData_size[(sizeof(CitadelSlaughterHordeContainModuleData) == 0x110) ? 1 : -1];
typedef char CitadelSlaughterHordeContainModuleData_off_m_statusForRingEntry[(offsetof(CitadelSlaughterHordeContainModuleData, m_statusForRingEntry) == 0xEC) ? 1 : -1];
typedef char CitadelSlaughterHordeContainModuleData_off_m_objectToDestroyForRingEntry[(offsetof(CitadelSlaughterHordeContainModuleData, m_objectToDestroyForRingEntry) == 0xFC) ? 1 : -1];
typedef char CitadelSlaughterHordeContainModuleData_off_m_upgradeForRingEntry[(offsetof(CitadelSlaughterHordeContainModuleData, m_upgradeForRingEntry) == 0x100) ? 1 : -1];
typedef char CitadelSlaughterHordeContainModuleData_off_m_fxForRingEntry[(offsetof(CitadelSlaughterHordeContainModuleData, m_fxForRingEntry) == 0x10C) ? 1 : -1];

#endif
