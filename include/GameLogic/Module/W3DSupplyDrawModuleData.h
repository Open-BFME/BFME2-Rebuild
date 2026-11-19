// W3DSupplyDrawModuleData.h: the INI data of W3DSupplyDraw. SIZE 0x18C.
// Target facts: ModuleFactory registers the module with operator new 0x18C and
// constructor RVA 0x000CAF17; buildFieldParse RVA 0x000CAE28 calls W3DModelDrawModuleData::buildFieldParse, then adds VA 0x00BCBED8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_W3_D_SUPPLY_DRAW_MODULE_DATA_H
#define BFME2_W3_D_SUPPLY_DRAW_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/W3DModelDrawModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base W3DModelDrawModuleData: the constructor calls its constructor on this. It ends at 0x188 (its own operator-new size);
// this class's members follow it.
typedef char W3DSupplyDrawModuleData_base_size[(sizeof(W3DModelDrawModuleData) == 0x188) ? 1 : -1];

class W3DSupplyDrawModuleData : public W3DModelDrawModuleData
{
public:
	W3DSupplyDrawModuleData();
	virtual ~W3DSupplyDrawModuleData();	// out of line in retail: 0x000CAF30 (the ModuleFactory deletes through slot 0)
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_supplyBonePrefix;                        // +0x188 SupplyBonePrefix
};
// SIZE 0x18C
typedef char W3DSupplyDrawModuleData_size[(sizeof(W3DSupplyDrawModuleData) == 0x18C) ? 1 : -1];
typedef char W3DSupplyDrawModuleData_off_m_supplyBonePrefix[(offsetof(W3DSupplyDrawModuleData, m_supplyBonePrefix) == 0x188) ? 1 : -1];

#endif
