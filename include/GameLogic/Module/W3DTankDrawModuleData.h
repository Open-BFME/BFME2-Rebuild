// W3DTankDrawModuleData.h: the INI data of W3DTankDraw. SIZE 0x19C.
// Target facts: ModuleFactory registers the module with operator new 0x19C and
// constructor RVA 0x000CE6E6; buildFieldParse RVA 0x000CDF96 calls W3DModelDrawModuleData::buildFieldParse, then adds VA 0x00BCCAD0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_W3_D_TANK_DRAW_MODULE_DATA_H
#define BFME2_W3_D_TANK_DRAW_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/W3DModelDrawModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base W3DModelDrawModuleData: the constructor calls its constructor on this. It ends at 0x188 (its own operator-new size);
// this class's members follow it.
typedef char W3DTankDrawModuleData_base_size[(sizeof(W3DModelDrawModuleData) == 0x188) ? 1 : -1];

class W3DTankDrawModuleData : public W3DModelDrawModuleData
{
public:
	W3DTankDrawModuleData();
	virtual ~W3DTankDrawModuleData();	// out of line in retail (ModuleFactory deletes through slot 0)
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_treadDebrisLeft;                         // +0x188 TreadDebrisLeft
	AsciiString m_treadDebrisRight;                        // +0x18C TreadDebrisRight
	Real m_treadAnimationRate;                             // +0x190 TreadAnimationRate; velocity
	Real m_treadPivotSpeedFraction;                        // +0x194 TreadPivotSpeedFraction
	Real m_treadDriveSpeedFraction;                        // +0x198 TreadDriveSpeedFraction
};
// SIZE 0x19C
typedef char W3DTankDrawModuleData_size[(sizeof(W3DTankDrawModuleData) == 0x19C) ? 1 : -1];
typedef char W3DTankDrawModuleData_off_m_treadDebrisLeft[(offsetof(W3DTankDrawModuleData, m_treadDebrisLeft) == 0x188) ? 1 : -1];
typedef char W3DTankDrawModuleData_off_m_treadDebrisRight[(offsetof(W3DTankDrawModuleData, m_treadDebrisRight) == 0x18C) ? 1 : -1];
typedef char W3DTankDrawModuleData_off_m_treadAnimationRate[(offsetof(W3DTankDrawModuleData, m_treadAnimationRate) == 0x190) ? 1 : -1];
typedef char W3DTankDrawModuleData_off_m_treadPivotSpeedFraction[(offsetof(W3DTankDrawModuleData, m_treadPivotSpeedFraction) == 0x194) ? 1 : -1];
typedef char W3DTankDrawModuleData_off_m_treadDriveSpeedFraction[(offsetof(W3DTankDrawModuleData, m_treadDriveSpeedFraction) == 0x198) ? 1 : -1];

#endif
