// TemporarilyDefectUpdateModuleData.h: the INI data of TemporarilyDefectUpdate. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA ?; buildFieldParse RVA 0x004CC69E adds VA 0x00C5F5E4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_TEMPORARILY_DEFECT_UPDATE_MODULE_DATA_H
#define BFME2_TEMPORARILY_DEFECT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class TemporarilyDefectUpdateModuleData : public ModuleData
{
public:
	TemporarilyDefectUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_defectDuration;                          // +0x8 DefectDuration; duration, frames
};
// SIZE 0xC
typedef char TemporarilyDefectUpdateModuleData_size[(sizeof(TemporarilyDefectUpdateModuleData) == 0xC) ? 1 : -1];
typedef char TemporarilyDefectUpdateModuleData_off_m_defectDuration[(offsetof(TemporarilyDefectUpdateModuleData, m_defectDuration) == 0x8) ? 1 : -1];

#endif
