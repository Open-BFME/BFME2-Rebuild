// InstantDeathBehaviorModuleData.h: the INI data of InstantDeathBehavior. SIZE 0x68.
// Target facts: ModuleFactory registers the module data with operator new 0x68 and constructor
// 0x0045D176, which calls DestroyDieModuleData's constructor on this object and then builds four
// 12-byte STLport vectors: FX at +0x38, OCL at +0x44, Weapon at +0x50 and Sound at +0x5C.
// The FX and Sound parsers fill theirs through the shared list parser and the module's own Sound
// parser; the OCL and Weapon parsers fill theirs with the names of the lists they read.
// Element types here are what onDie reads back. Layout only: the parse functions are not declared.
#pragma once
#ifndef BFME2_INSTANT_DEATH_BEHAVIOR_MODULE_DATA_H
#define BFME2_INSTANT_DEATH_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/OpaqueRefElement4.h"
#include "Common/Vector.h"
#include "GameLogic/Module/DestroyDieModuleData.h"

class FXList;
class ObjectCreationList;
class WeaponTemplate;

// DestroyDieModuleData ends at 0x38; this class's four vectors follow it.
typedef char InstantDeathBehaviorModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class InstantDeathBehaviorModuleData : public DestroyDieModuleData
{
public:
	InstantDeathBehaviorModuleData();									// 0x0045D176
	virtual ~InstantDeathBehaviorModuleData();

	_STL::vector<const FXList *> m_fx;									// +0x38 FX
	_STL::vector<ObjectCreationList *> m_ocls;      					// +0x44 OCL
	_STL::vector<const WeaponTemplate *> m_weapons;						// +0x50 Weapon
	_STL::vector<OpaqueRefElement4> m_sounds;							// +0x5C Sound
};
// SIZE 0x68
typedef char InstantDeathBehaviorModuleData_size[(sizeof(InstantDeathBehaviorModuleData) == 0x68) ? 1 : -1];
typedef char InstantDeathBehaviorModuleData_off_m_fx[(offsetof(InstantDeathBehaviorModuleData, m_fx) == 0x38) ? 1 : -1];

#endif
