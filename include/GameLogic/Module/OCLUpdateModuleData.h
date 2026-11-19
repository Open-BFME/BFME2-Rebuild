// OCLUpdateModuleData.h: the INI data of OCLUpdate. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x0049B355; buildFieldParse RVA 0x0049B370 adds VA 0x00C50BA8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_O_C_L_UPDATE_MODULE_DATA_H
#define BFME2_O_C_L_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;
class ObjectCreationList;

class OCLUpdateModuleData : public ModuleData
{
public:
	OCLUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	const ObjectCreationList * m_ocl;                      // +0x8 OCL
	UnsignedInt m_minDelay;                                // +0xC MinDelay; duration, frames
	UnsignedInt m_maxDelay;                                // +0x10 MaxDelay; duration, frames
	Int m_amount;                                          // +0x14 Amount
	Bool m_createAtEdge;                                   // +0x18 CreateAtEdge
};
// SIZE 0x1C
typedef char OCLUpdateModuleData_size[(sizeof(OCLUpdateModuleData) == 0x1C) ? 1 : -1];
typedef char OCLUpdateModuleData_off_m_ocl[(offsetof(OCLUpdateModuleData, m_ocl) == 0x8) ? 1 : -1];
typedef char OCLUpdateModuleData_off_m_minDelay[(offsetof(OCLUpdateModuleData, m_minDelay) == 0xC) ? 1 : -1];
typedef char OCLUpdateModuleData_off_m_maxDelay[(offsetof(OCLUpdateModuleData, m_maxDelay) == 0x10) ? 1 : -1];
typedef char OCLUpdateModuleData_off_m_amount[(offsetof(OCLUpdateModuleData, m_amount) == 0x14) ? 1 : -1];
typedef char OCLUpdateModuleData_off_m_createAtEdge[(offsetof(OCLUpdateModuleData, m_createAtEdge) == 0x18) ? 1 : -1];

#endif
