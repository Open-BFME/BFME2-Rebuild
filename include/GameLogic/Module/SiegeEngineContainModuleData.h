// SiegeEngineContainModuleData.h: the INI data of SiegeEngineContain. SIZE 0x1B8.
// Target facts: ModuleFactory registers the module with operator new 0x1B8 and
// constructor RVA 0x0047C927; buildFieldParse RVA 0x0047B9F5 calls TransportContainModuleData::buildFieldParse, then adds VA 0x00C46EC8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SIEGE_ENGINE_CONTAIN_MODULE_DATA_H
#define BFME2_SIEGE_ENGINE_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/BitFlags.h"
#include "Common/GameType.h"
#include "GameLogic/Module/TransportContainModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

// Base TransportContainModuleData: the constructor calls its constructor on this. It ends at 0x18C (its own operator-new size);
// this class's members follow it.
typedef char SiegeEngineContainModuleData_base_size[(sizeof(TransportContainModuleData) == 0x18C) ? 1 : -1];

class SiegeEngineContainModuleData : public TransportContainModuleData
{
public:
	SiegeEngineContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static void parseInitialCrew(INI *ini, void *instance, void *store, const void *userData);

	ObjectFilter m_crewFilter;                             // +0x18C CrewFilter
	Int m_crewMax;                                         // +0x190 CrewMax
	UnsignedByte m_unk0x194[0x8];
	Real m_speedPercentPerCrew;                            // +0x19C SpeedPercentPerCrew; percent
	Bool m_crewAllowedToFire;                              // +0x1A0 CrewAllowedToFire
	UnsignedByte m_unk0x1A1[0x3];
	ObjectStatusMaskType m_objectStatusOfCrew;	// Parsed by the shared containment object-status parser.
	Bool m_transferSelection;                              // +0x1B4 TransferSelection
};
// SIZE 0x1B8
typedef char SiegeEngineContainModuleData_size[(sizeof(SiegeEngineContainModuleData) == 0x1B8) ? 1 : -1];
typedef char SiegeEngineContainModuleData_off_m_crewFilter[(offsetof(SiegeEngineContainModuleData, m_crewFilter) == 0x18C) ? 1 : -1];
typedef char SiegeEngineContainModuleData_off_m_crewMax[(offsetof(SiegeEngineContainModuleData, m_crewMax) == 0x190) ? 1 : -1];
typedef char SiegeEngineContainModuleData_off_m_speedPercentPerCrew[(offsetof(SiegeEngineContainModuleData, m_speedPercentPerCrew) == 0x19C) ? 1 : -1];
typedef char SiegeEngineContainModuleData_off_m_crewAllowedToFire[(offsetof(SiegeEngineContainModuleData, m_crewAllowedToFire) == 0x1A0) ? 1 : -1];
typedef char SiegeEngineContainModuleData_off_m_objectStatusOfCrew[(offsetof(SiegeEngineContainModuleData, m_objectStatusOfCrew) == 0x1A4) ? 1 : -1];
typedef char SiegeEngineContainModuleData_off_m_transferSelection[(offsetof(SiegeEngineContainModuleData, m_transferSelection) == 0x1B4) ? 1 : -1];

#endif
