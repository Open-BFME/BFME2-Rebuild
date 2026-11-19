// BoredUpdateModuleData.h: the INI data of BoredUpdate. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x004966FE; buildFieldParse RVA 0x00496780 adds VA 0x00C4F618 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_BORED_UPDATE_MODULE_DATA_H
#define BFME2_BORED_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class SpecialPowerTemplate;

class BoredUpdateModuleData : public ModuleData
{
public:
	BoredUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_scanDelayTime;                           // +0x8 ScanDelayTime; duration, frames
	Real m_scanDistance;                                   // +0xC ScanDistance
	ObjectFilter m_boredFilter;                            // +0x10 BoredFilter
	Bool m_canScanWhileAttackingOrMoving;                  // +0x14 CanScanWhileAttackingOrMoving
	const SpecialPowerTemplate * m_specialPowerTemplate;   // +0x18 SpecialPowerTemplate
};
// SIZE 0x1C
typedef char BoredUpdateModuleData_size[(sizeof(BoredUpdateModuleData) == 0x1C) ? 1 : -1];
typedef char BoredUpdateModuleData_off_m_scanDelayTime[(offsetof(BoredUpdateModuleData, m_scanDelayTime) == 0x8) ? 1 : -1];
typedef char BoredUpdateModuleData_off_m_scanDistance[(offsetof(BoredUpdateModuleData, m_scanDistance) == 0xC) ? 1 : -1];
typedef char BoredUpdateModuleData_off_m_boredFilter[(offsetof(BoredUpdateModuleData, m_boredFilter) == 0x10) ? 1 : -1];
typedef char BoredUpdateModuleData_off_m_canScanWhileAttackingOrMoving[(offsetof(BoredUpdateModuleData, m_canScanWhileAttackingOrMoving) == 0x14) ? 1 : -1];
typedef char BoredUpdateModuleData_off_m_specialPowerTemplate[(offsetof(BoredUpdateModuleData, m_specialPowerTemplate) == 0x18) ? 1 : -1];

#endif
