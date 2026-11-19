// W3DBoatWakeModelDrawModuleData.h: the INI data of W3DBoatWakeModelDraw. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x000D0CD8; buildFieldParse RVA 0x000D0CF7 adds VA 0x00BCDEE4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_W3_D_BOAT_WAKE_MODEL_DRAW_MODULE_DATA_H
#define BFME2_W3_D_BOAT_WAKE_MODEL_DRAW_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"

class INI;
class MultiIniFieldParse;

class W3DBoatWakeModelDrawModuleData : public ModuleData
{
public:
	W3DBoatWakeModelDrawModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_modelName;                               // +0x8 ModelName
	Coord3D m_offset;                                      // +0xC Offset
};
// SIZE 0x18
typedef char W3DBoatWakeModelDrawModuleData_size[(sizeof(W3DBoatWakeModelDrawModuleData) == 0x18) ? 1 : -1];
typedef char W3DBoatWakeModelDrawModuleData_off_m_modelName[(offsetof(W3DBoatWakeModelDrawModuleData, m_modelName) == 0x8) ? 1 : -1];
typedef char W3DBoatWakeModelDrawModuleData_off_m_offset[(offsetof(W3DBoatWakeModelDrawModuleData, m_offset) == 0xC) ? 1 : -1];

#endif
