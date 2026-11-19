// HordeTransportContainModuleData.h: the INI data of HordeTransportContain. SIZE 0x18C.
// Target facts: ModuleFactory registers the module with operator new 0x18C and
// constructor RVA 0x00477D61; buildFieldParse RVA 0x00476FFE calls TransportContainModuleData::buildFieldParse, then adds no own table.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_HORDE_TRANSPORT_CONTAIN_MODULE_DATA_H
#define BFME2_HORDE_TRANSPORT_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/TransportContainModuleData.h"

class INI;
class MultiIniFieldParse;

// Base TransportContainModuleData: the constructor calls its constructor on this. It ends at 0x18C (its own operator-new size);
// this class's members follow it.
typedef char HordeTransportContainModuleData_base_size[(sizeof(TransportContainModuleData) == 0x18C) ? 1 : -1];

class HordeTransportContainModuleData : public TransportContainModuleData
{
public:
	HordeTransportContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

};
// SIZE 0x18C
typedef char HordeTransportContainModuleData_size[(sizeof(HordeTransportContainModuleData) == 0x18C) ? 1 : -1];

#endif
