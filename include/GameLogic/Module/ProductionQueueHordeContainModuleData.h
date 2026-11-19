// ProductionQueueHordeContainModuleData.h: the INI data of ProductionQueueHordeContain. SIZE 0xE0.
// Target facts: ModuleFactory registers the module with operator new 0xE0 and
// constructor RVA 0x00481776; buildFieldParse RVA 0x0048175B calls HordeGarrisonContainModuleData::buildFieldParse, then adds VA 0x00C490D4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_destinationTemplate +0xD4: ProductionQueueHordeContainModuleData::iniParseTemplateContainer 0x004816F4
#pragma once
#ifndef BFME2_PRODUCTION_QUEUE_HORDE_CONTAIN_MODULE_DATA_H
#define BFME2_PRODUCTION_QUEUE_HORDE_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/HordeGarrisonContainModuleData.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;

// One DestinationTemplate entry: 8 bytes (the vector stride). Its two members, an object filter at +0 and
// a name at +4, are not typed yet.
struct ProductionQueueDestinationEntry
{
	unsigned char m_unk0x00[8];
};

// Base HordeGarrisonContainModuleData: the constructor calls its constructor on this. It ends at 0xD4 (its own operator-new size);
// this class's members follow it.
typedef char ProductionQueueHordeContainModuleData_base_size[(sizeof(HordeGarrisonContainModuleData) == 0xD4) ? 1 : -1];

class ProductionQueueHordeContainModuleData : public HordeGarrisonContainModuleData
{
public:
	ProductionQueueHordeContainModuleData();
	virtual ~ProductionQueueHordeContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);
	static void iniParseTemplateContainer(INI *ini, void *instance, void *store, const void *userData);

	_STL::vector<ProductionQueueDestinationEntry> m_destinationTemplate;   // +0xD4 DestinationTemplate (0xC bytes); parsed by iniParseTemplateContainer
};
// SIZE 0xE0
typedef char ProductionQueueHordeContainModuleData_size[(sizeof(ProductionQueueHordeContainModuleData) == 0xE0) ? 1 : -1];
typedef char ProductionQueueHordeContainModuleData_off_m_destinationTemplate[(offsetof(ProductionQueueHordeContainModuleData, m_destinationTemplate) == 0xD4) ? 1 : -1];

#endif
