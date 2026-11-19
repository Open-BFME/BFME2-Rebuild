// W3DBuffDrawModuleData.h: the INI data of W3DBuffDraw. SIZE 0x10.
// Target facts: ModuleFactory registers the module with operator new 0x10 and
// constructor RVA 0x000CEE4C; buildFieldParse RVA 0x000CEE93 adds VA 0x00BCD2F4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_W3_D_BUFF_DRAW_MODULE_DATA_H
#define BFME2_W3_D_BUFF_DRAW_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class W3DBuffDrawModuleData : public ModuleData
{
public:
	W3DBuffDrawModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_modelName;                               // +0x8 ModelName
	Bool m_preDraw;                                        // +0xC PreDraw
};
// SIZE 0x10
typedef char W3DBuffDrawModuleData_size[(sizeof(W3DBuffDrawModuleData) == 0x10) ? 1 : -1];
typedef char W3DBuffDrawModuleData_off_m_modelName[(offsetof(W3DBuffDrawModuleData, m_modelName) == 0x8) ? 1 : -1];
typedef char W3DBuffDrawModuleData_off_m_preDraw[(offsetof(W3DBuffDrawModuleData, m_preDraw) == 0xC) ? 1 : -1];

#endif
