// CrateCollideModuleData.h: the INI data of ShroudCrateCollide. SIZE 0x5C.
// Target facts: ModuleFactory registers the module with operator new 0x5C and
// constructor RVA 0x004BC657; buildFieldParse RVA 0x004BC7FE adds VA 0x00C5A7A8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CRATE_COLLIDE_MODULE_DATA_H
#define BFME2_CRATE_COLLIDE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/BitFlags.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;

class CrateCollideModuleData : public ModuleData
{
public:
	CrateCollideModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	KindOfMaskType m_requiredKindOf;                       // +0x8 RequiredKindOf; BitFlags<218>
	KindOfMaskType m_forbiddenKindOf;                      // +0x24 ForbiddenKindOf; BitFlags<218>
	Bool m_forbidOwnerPlayer;                              // +0x40 ForbidOwnerPlayer
	Bool m_buildingPickup;                                 // +0x41 BuildingPickup
	Bool m_humanOnly;                                      // +0x42 HumanOnly
	Int m_pickupScience;                                   // +0x44 PickupScience; ScienceType
	const FXList * m_executeFX;                            // +0x48 ExecuteFX
	AsciiString m_executeAnimation;                        // +0x4C ExecuteAnimation
	Real m_executeAnimationTime;                           // +0x50 ExecuteAnimationTime
	Real m_executeAnimationZRise;                          // +0x54 ExecuteAnimationZRise
	Bool m_executeAnimationFades;                          // +0x58 ExecuteAnimationFades
};
// SIZE 0x5C
typedef char CrateCollideModuleData_size[(sizeof(CrateCollideModuleData) == 0x5C) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_requiredKindOf[(offsetof(CrateCollideModuleData, m_requiredKindOf) == 0x8) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_forbiddenKindOf[(offsetof(CrateCollideModuleData, m_forbiddenKindOf) == 0x24) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_forbidOwnerPlayer[(offsetof(CrateCollideModuleData, m_forbidOwnerPlayer) == 0x40) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_buildingPickup[(offsetof(CrateCollideModuleData, m_buildingPickup) == 0x41) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_humanOnly[(offsetof(CrateCollideModuleData, m_humanOnly) == 0x42) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_pickupScience[(offsetof(CrateCollideModuleData, m_pickupScience) == 0x44) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_executeFX[(offsetof(CrateCollideModuleData, m_executeFX) == 0x48) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_executeAnimation[(offsetof(CrateCollideModuleData, m_executeAnimation) == 0x4C) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_executeAnimationTime[(offsetof(CrateCollideModuleData, m_executeAnimationTime) == 0x50) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_executeAnimationZRise[(offsetof(CrateCollideModuleData, m_executeAnimationZRise) == 0x54) ? 1 : -1];
typedef char CrateCollideModuleData_off_m_executeAnimationFades[(offsetof(CrateCollideModuleData, m_executeAnimationFades) == 0x58) ? 1 : -1];

#endif
