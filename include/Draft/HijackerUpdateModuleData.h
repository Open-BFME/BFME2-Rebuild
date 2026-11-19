// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF2318).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// HijackerUpdateModuleData.h: the INI data of HijackerUpdate. SIZE 0x10.
// Target facts: ModuleFactory registers the module with operator new 0x10 and
// constructor RVA ?; buildFieldParse RVA 0x0025437B adds VA 0x00BF2318 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_HIJACKER_UPDATE_MODULE_DATA_H
#define BFME2_HIJACKER_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class HijackerUpdateModuleData : public ModuleData
{
public:
	HijackerUpdateModuleData() {}
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_attachToTargetBone;                      // +0x8 AttachToTargetBone
	AsciiString m_parachuteName;                           // +0xC ParachuteName
};
// SIZE 0x10
typedef char HijackerUpdateModuleData_size[(sizeof(HijackerUpdateModuleData) == 0x10) ? 1 : -1];
typedef char HijackerUpdateModuleData_off_m_attachToTargetBone[(offsetof(HijackerUpdateModuleData, m_attachToTargetBone) == 0x8) ? 1 : -1];
typedef char HijackerUpdateModuleData_off_m_parachuteName[(offsetof(HijackerUpdateModuleData, m_parachuteName) == 0xC) ? 1 : -1];

#endif
