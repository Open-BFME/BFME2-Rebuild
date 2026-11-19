// HordeSiegeEngineContainModuleData.h: the INI data of HordeSiegeEngineContain. SIZE 0x1B4.
// Target facts: ModuleFactory registers the module with operator new 0x1B4 and
// constructor RVA 0x0047DA08; buildFieldParse RVA 0x0047CA32 calls HordeTransportContainModuleData::buildFieldParse, then adds VA 0x00C47208 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_HORDE_SIEGE_ENGINE_CONTAIN_MODULE_DATA_H
#define BFME2_HORDE_SIEGE_ENGINE_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/BitFlags.h"
#include "Common/GameType.h"
#include "GameLogic/Module/HordeTransportContainModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

// Base HordeTransportContainModuleData: the constructor calls its constructor on this. It ends at 0x18C (its own operator-new size);
// this class's members follow it.
typedef char HordeSiegeEngineContainModuleData_base_size[(sizeof(HordeTransportContainModuleData) == 0x18C) ? 1 : -1];

class HordeSiegeEngineContainModuleData : public HordeTransportContainModuleData
{
public:
	HordeSiegeEngineContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	ObjectFilter m_crewFilter;                             // +0x18C CrewFilter
	Int m_crewMax;                                         // +0x190 CrewMax
	UnsignedByte m_unk0x194[0x8];
	Real m_speedPercentPerCrew;                            // +0x19C SpeedPercentPerCrew; percent
	Bool m_crewAllowedToFire;                              // +0x1A0 CrewAllowedToFire
	UnsignedByte m_unk0x1A1[0x3];
	ObjectStatusMaskType m_objectStatusOfCrew;	// Parsed by the shared containment object-status parser.
};
// SIZE 0x1B4
typedef char HordeSiegeEngineContainModuleData_size[(sizeof(HordeSiegeEngineContainModuleData) == 0x1B4) ? 1 : -1];
typedef char HordeSiegeEngineContainModuleData_off_m_crewFilter[(offsetof(HordeSiegeEngineContainModuleData, m_crewFilter) == 0x18C) ? 1 : -1];
typedef char HordeSiegeEngineContainModuleData_off_m_crewMax[(offsetof(HordeSiegeEngineContainModuleData, m_crewMax) == 0x190) ? 1 : -1];
typedef char HordeSiegeEngineContainModuleData_off_m_speedPercentPerCrew[(offsetof(HordeSiegeEngineContainModuleData, m_speedPercentPerCrew) == 0x19C) ? 1 : -1];
typedef char HordeSiegeEngineContainModuleData_off_m_crewAllowedToFire[(offsetof(HordeSiegeEngineContainModuleData, m_crewAllowedToFire) == 0x1A0) ? 1 : -1];
typedef char HordeSiegeEngineContainModuleData_off_m_objectStatusOfCrew[(offsetof(HordeSiegeEngineContainModuleData, m_objectStatusOfCrew) == 0x1A4) ? 1 : -1];

#endif
