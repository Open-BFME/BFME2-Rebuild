// RebuildHoleExposeDieModuleData.h: the INI data of RebuildHoleExposeDie. SIZE 0x48.
// Target facts: ModuleFactory registers the module with operator new 0x48 and
// constructor RVA 0x0048671F; buildFieldParse RVA 0x00486746 adds VA 0x00C4AE00 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_REBUILD_HOLE_EXPOSE_DIE_MODULE_DATA_H
#define BFME2_REBUILD_HOLE_EXPOSE_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char RebuildHoleExposeDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class RebuildHoleExposeDieModuleData : public DestroyDieModuleData
{
public:
	RebuildHoleExposeDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_holeName;                                // +0x38 HoleName
	Real m_holeMaxHealth;                                  // +0x3C HoleMaxHealth
	Real m_fadeInTimeSeconds;                              // +0x40 FadeInTimeSeconds
	Bool m_transferAttackers;                              // +0x44 TransferAttackers
};
// SIZE 0x48
typedef char RebuildHoleExposeDieModuleData_size[(sizeof(RebuildHoleExposeDieModuleData) == 0x48) ? 1 : -1];
typedef char RebuildHoleExposeDieModuleData_off_m_holeName[(offsetof(RebuildHoleExposeDieModuleData, m_holeName) == 0x38) ? 1 : -1];
typedef char RebuildHoleExposeDieModuleData_off_m_holeMaxHealth[(offsetof(RebuildHoleExposeDieModuleData, m_holeMaxHealth) == 0x3C) ? 1 : -1];
typedef char RebuildHoleExposeDieModuleData_off_m_fadeInTimeSeconds[(offsetof(RebuildHoleExposeDieModuleData, m_fadeInTimeSeconds) == 0x40) ? 1 : -1];
typedef char RebuildHoleExposeDieModuleData_off_m_transferAttackers[(offsetof(RebuildHoleExposeDieModuleData, m_transferAttackers) == 0x44) ? 1 : -1];

#endif
