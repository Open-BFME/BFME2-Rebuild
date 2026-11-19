// TaintSpecialPowerModuleData.h: the INI data of TaintSpecialPower. SIZE 0x8C.
// Target facts: ModuleFactory registers the module with operator new 0x8C and
// constructor RVA 0x004C4AB8; buildFieldParse RVA 0x004C487B calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5D518 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_TAINT_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_TAINT_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModule.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char TaintSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class TaintSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	TaintSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_taintObject;                             // +0x7C TaintObject
	Real m_taintRadius;                                    // +0x80 TaintRadius
	const FXList * m_taintFX;                              // +0x84 TaintFX
	const ObjectCreationList * m_taintOCL;                 // +0x88 TaintOCL
};
// SIZE 0x8C
typedef char TaintSpecialPowerModuleData_size[(sizeof(TaintSpecialPowerModuleData) == 0x8C) ? 1 : -1];
typedef char TaintSpecialPowerModuleData_off_m_taintObject[(offsetof(TaintSpecialPowerModuleData, m_taintObject) == 0x7C) ? 1 : -1];
typedef char TaintSpecialPowerModuleData_off_m_taintRadius[(offsetof(TaintSpecialPowerModuleData, m_taintRadius) == 0x80) ? 1 : -1];
typedef char TaintSpecialPowerModuleData_off_m_taintFX[(offsetof(TaintSpecialPowerModuleData, m_taintFX) == 0x84) ? 1 : -1];
typedef char TaintSpecialPowerModuleData_off_m_taintOCL[(offsetof(TaintSpecialPowerModuleData, m_taintOCL) == 0x88) ? 1 : -1];

#endif
