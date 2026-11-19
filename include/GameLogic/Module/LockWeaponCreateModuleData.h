// LockWeaponCreateModuleData.h: the INI data of LockWeaponCreate. SIZE 0xC.
// Proven by buildFieldParse 0x004B8C9C (h06). Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA 0x00254C8E; buildFieldParse RVA 0x004B8C9C adds VA 0x00C59338 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_LOCK_WEAPON_CREATE_MODULE_DATA_H
#define BFME2_LOCK_WEAPON_CREATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class LockWeaponCreateModuleData : public ModuleData
{
public:
	LockWeaponCreateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_slotToLock;                                      // +0x8 SlotToLock; lookup-list value (enum)
};
// SIZE 0xC
typedef char LockWeaponCreateModuleData_size[(sizeof(LockWeaponCreateModuleData) == 0xC) ? 1 : -1];
typedef char LockWeaponCreateModuleData_off_m_slotToLock[(offsetof(LockWeaponCreateModuleData, m_slotToLock) == 0x8) ? 1 : -1];

#endif
