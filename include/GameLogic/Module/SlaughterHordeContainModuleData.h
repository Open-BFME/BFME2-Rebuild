// SlaughterHordeContainModuleData.h: the INI data of SlaughterHordeContain. SIZE 0xEC.
// Target facts: ModuleFactory registers the module with operator new 0xEC and
// constructor RVA 0x0048104D; buildFieldParse RVA 0x00480266 calls HordeGarrisonContainModuleData::buildFieldParse, then adds VA 0x00C487F8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SLAUGHTER_HORDE_CONTAIN_MODULE_DATA_H
#define BFME2_SLAUGHTER_HORDE_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/HordeGarrisonContainModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;

// Base HordeGarrisonContainModuleData: the constructor calls its constructor on this. It ends at 0xD4 (its own operator-new size);
// this class's members follow it.
typedef char SlaughterHordeContainModuleData_base_size[(sizeof(HordeGarrisonContainModuleData) == 0xD4) ? 1 : -1];

class SlaughterHordeContainModuleData : public HordeGarrisonContainModuleData
{
public:
	SlaughterHordeContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_cashBackPercent;                                // +0xD4 CashBackPercent; percent
	ObjectFilter m_canAlwaysEnter;                         // +0xD8 CanAlwaysEnter
	ObjectStatusMaskType m_statusRequiredForCanAlwaysEnter;// +0xDC StatusRequiredForCanAlwaysEnter; BitFlags<101>
};
// SIZE 0xEC
typedef char SlaughterHordeContainModuleData_size[(sizeof(SlaughterHordeContainModuleData) == 0xEC) ? 1 : -1];
typedef char SlaughterHordeContainModuleData_off_m_cashBackPercent[(offsetof(SlaughterHordeContainModuleData, m_cashBackPercent) == 0xD4) ? 1 : -1];
typedef char SlaughterHordeContainModuleData_off_m_canAlwaysEnter[(offsetof(SlaughterHordeContainModuleData, m_canAlwaysEnter) == 0xD8) ? 1 : -1];
typedef char SlaughterHordeContainModuleData_off_m_statusRequiredForCanAlwaysEnter[(offsetof(SlaughterHordeContainModuleData, m_statusRequiredForCanAlwaysEnter) == 0xDC) ? 1 : -1];

#endif
