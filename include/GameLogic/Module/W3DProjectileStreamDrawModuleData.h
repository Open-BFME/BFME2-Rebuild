// W3DProjectileStreamDrawModuleData.h: the INI data of W3DProjectileStreamDraw. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x000D11D2; buildFieldParse RVA 0x000D125C adds VA 0x00BCDFB0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_W3_D_PROJECTILE_STREAM_DRAW_MODULE_DATA_H
#define BFME2_W3_D_PROJECTILE_STREAM_DRAW_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class W3DProjectileStreamDrawModuleData : public ModuleData
{
public:
	W3DProjectileStreamDrawModuleData();                   // 0x000D11D2
	virtual ~W3DProjectileStreamDrawModuleData();          // 0x000D1226
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_texture;                                 // +0x8 Texture
	Real m_width;                                          // +0xC Width
	Real m_tileFactor;                                     // +0x10 TileFactor
	Real m_scrollRate;                                     // +0x14 ScrollRate
	Int m_maxSegments;                                     // +0x18 MaxSegments
};
// SIZE 0x1C
typedef char W3DProjectileStreamDrawModuleData_size[(sizeof(W3DProjectileStreamDrawModuleData) == 0x1C) ? 1 : -1];
typedef char W3DProjectileStreamDrawModuleData_off_m_texture[(offsetof(W3DProjectileStreamDrawModuleData, m_texture) == 0x8) ? 1 : -1];
typedef char W3DProjectileStreamDrawModuleData_off_m_width[(offsetof(W3DProjectileStreamDrawModuleData, m_width) == 0xC) ? 1 : -1];
typedef char W3DProjectileStreamDrawModuleData_off_m_tileFactor[(offsetof(W3DProjectileStreamDrawModuleData, m_tileFactor) == 0x10) ? 1 : -1];
typedef char W3DProjectileStreamDrawModuleData_off_m_scrollRate[(offsetof(W3DProjectileStreamDrawModuleData, m_scrollRate) == 0x14) ? 1 : -1];
typedef char W3DProjectileStreamDrawModuleData_off_m_maxSegments[(offsetof(W3DProjectileStreamDrawModuleData, m_maxSegments) == 0x18) ? 1 : -1];

#endif
