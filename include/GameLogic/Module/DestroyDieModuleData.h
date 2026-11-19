// DestroyDieModuleData.h: the INI data of DestroyDie. SIZE 0x38.
// Target facts: ModuleFactory registers the module with operator new 0x38 and
// constructor RVA 0x00253510; buildFieldParse RVA 0x002534FE adds no own table and DieMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DESTROY_DIE_MODULE_DATA_H
#define BFME2_DESTROY_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/DieMuxData.h"

class INI;
class MultiIniFieldParse;

class DestroyDieModuleData : public ModuleData
{
public:
	DestroyDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	DieMuxData m_dieMuxData;                               // +0x8; added by buildFieldParse at +0x8
};
// SIZE 0x38
typedef char DestroyDieModuleData_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];
typedef char DestroyDieModuleData_off_m_dieMuxData[(offsetof(DestroyDieModuleData, m_dieMuxData) == 0x8) ? 1 : -1];

#endif
