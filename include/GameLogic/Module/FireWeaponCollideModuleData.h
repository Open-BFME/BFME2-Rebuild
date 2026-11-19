// FireWeaponCollideModuleData.h: the INI data of FireWeaponCollide. SIZE 0x30.
// Target facts: ModuleFactory registers the module with operator new 0x30 and
// constructor RVA 0x00254A36; buildFieldParse RVA 0x004BB918 adds VA 0x00C5A190 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_FIRE_WEAPON_COLLIDE_MODULE_DATA_H
#define BFME2_FIRE_WEAPON_COLLIDE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class WeaponTemplate;

class FireWeaponCollideModuleData : public ModuleData
{
public:
	FireWeaponCollideModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	const WeaponTemplate * m_collideWeapon;                // +0x8 CollideWeapon
	ObjectStatusMaskType m_requiredStatus;                 // +0xC RequiredStatus; BitFlags<101>
	ObjectStatusMaskType m_forbiddenStatus;                // +0x1C ForbiddenStatus; BitFlags<101>
	Bool m_fireOnce;                                       // +0x2C FireOnce
};
// SIZE 0x30
typedef char FireWeaponCollideModuleData_size[(sizeof(FireWeaponCollideModuleData) == 0x30) ? 1 : -1];
typedef char FireWeaponCollideModuleData_off_m_collideWeapon[(offsetof(FireWeaponCollideModuleData, m_collideWeapon) == 0x8) ? 1 : -1];
typedef char FireWeaponCollideModuleData_off_m_requiredStatus[(offsetof(FireWeaponCollideModuleData, m_requiredStatus) == 0xC) ? 1 : -1];
typedef char FireWeaponCollideModuleData_off_m_forbiddenStatus[(offsetof(FireWeaponCollideModuleData, m_forbiddenStatus) == 0x1C) ? 1 : -1];
typedef char FireWeaponCollideModuleData_off_m_fireOnce[(offsetof(FireWeaponCollideModuleData, m_fireOnce) == 0x2C) ? 1 : -1];

#endif
