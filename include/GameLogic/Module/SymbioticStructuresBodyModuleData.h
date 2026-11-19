// SymbioticStructuresBodyModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x00256D50);
// name, size, bases and virtual order are retail facts given below.
// SymbioticStructuresBodyModuleData.h: the INI data of SymbioticStructuresBody. SIZE 0x68.
// Target facts: ModuleFactory registers the module with operator new 0x68 and
// constructor RVA 0x00256D0D; buildFieldParse RVA 0x00251505 calls ActiveBodyModuleData::buildFieldParse, then adds VA 0x00BEFCD4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SYMBIOTIC_STRUCTURES_BODY_MODULE_DATA_H
#define BFME2_SYMBIOTIC_STRUCTURES_BODY_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBodyModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base ActiveBodyModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char SymbioticStructuresBodyModuleData_base_size[(sizeof(ActiveBodyModuleData) == 0x64) ? 1 : -1];

class SymbioticStructuresBodyModuleData : public ActiveBodyModuleData
{
public:
	SymbioticStructuresBodyModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x00256D50

	AsciiString m_symbiote;                                // +0x64 Symbiote
};
// SIZE 0x68
typedef char SymbioticStructuresBodyModuleData_size[(sizeof(SymbioticStructuresBodyModuleData) == 0x68) ? 1 : -1];
typedef char SymbioticStructuresBodyModuleData_off_m_symbiote[(offsetof(SymbioticStructuresBodyModuleData, m_symbiote) == 0x64) ? 1 : -1];

#endif
