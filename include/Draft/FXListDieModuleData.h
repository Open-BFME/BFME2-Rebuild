// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF08E0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// FXListDieModuleData.h: the INI data of FXListDie. SIZE 0x40.
// Target facts: ModuleFactory registers the module with operator new 0x40 and
// constructor RVA 0x00253A78; buildFieldParse RVA 0x00253A92 adds VA 0x00BF08E0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_F_X_LIST_DIE_MODULE_DATA_H
#define BFME2_F_X_LIST_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char FXListDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class FXListDieModuleData : public DestroyDieModuleData
{
public:
	FXListDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const FXList * m_deathFX;                              // +0x38 DeathFX
	Bool m_orientToObject;                                 // +0x3C OrientToObject
};
// SIZE 0x40
typedef char FXListDieModuleData_size[(sizeof(FXListDieModuleData) == 0x40) ? 1 : -1];
typedef char FXListDieModuleData_off_m_deathFX[(offsetof(FXListDieModuleData, m_deathFX) == 0x38) ? 1 : -1];
typedef char FXListDieModuleData_off_m_orientToObject[(offsetof(FXListDieModuleData, m_orientToObject) == 0x3C) ? 1 : -1];

#endif
