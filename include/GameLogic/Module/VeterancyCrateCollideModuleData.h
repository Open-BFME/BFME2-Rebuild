// VeterancyCrateCollideModuleData.h: the INI data of VeterancyCrateCollide. SIZE 0x68.
// Target facts: ModuleFactory registers the module with operator new 0x68 and
// constructor RVA 0x00255B09; buildFieldParse RVA 0x00251246 calls CrateCollideModuleData::buildFieldParse, then adds VA 0x00BEFB60 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_VETERANCY_CRATE_COLLIDE_MODULE_DATA_H
#define BFME2_VETERANCY_CRATE_COLLIDE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CrateCollideModuleData.h"

class INI;
class MultiIniFieldParse;

// Base CrateCollideModuleData: the constructor calls its constructor on this. It ends at 0x5C (its own operator-new size);
// this class's members follow it.
typedef char VeterancyCrateCollideModuleData_base_size[(sizeof(CrateCollideModuleData) == 0x5C) ? 1 : -1];

class VeterancyCrateCollideModuleData : public CrateCollideModuleData
{
public:
	VeterancyCrateCollideModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_effectRange;                             // +0x5C EffectRange
	Bool m_addsOwnerVeterancy;                             // +0x60 AddsOwnerVeterancy
	Bool m_isPilot;                                        // +0x61 IsPilot
	Int m_affectsUpToLevel;                                // +0x64 AffectsUpToLevel
};
// SIZE 0x68
typedef char VeterancyCrateCollideModuleData_size[(sizeof(VeterancyCrateCollideModuleData) == 0x68) ? 1 : -1];
typedef char VeterancyCrateCollideModuleData_off_m_effectRange[(offsetof(VeterancyCrateCollideModuleData, m_effectRange) == 0x5C) ? 1 : -1];
typedef char VeterancyCrateCollideModuleData_off_m_addsOwnerVeterancy[(offsetof(VeterancyCrateCollideModuleData, m_addsOwnerVeterancy) == 0x60) ? 1 : -1];
typedef char VeterancyCrateCollideModuleData_off_m_isPilot[(offsetof(VeterancyCrateCollideModuleData, m_isPilot) == 0x61) ? 1 : -1];
typedef char VeterancyCrateCollideModuleData_off_m_affectsUpToLevel[(offsetof(VeterancyCrateCollideModuleData, m_affectsUpToLevel) == 0x64) ? 1 : -1];

#endif
