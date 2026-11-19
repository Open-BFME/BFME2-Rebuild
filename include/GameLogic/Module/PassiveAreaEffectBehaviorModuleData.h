// PassiveAreaEffectBehaviorModuleData.h: the INI data of PassiveAreaEffectBehavior. SIZE 0x38.
// Target facts: ModuleFactory registers the module with operator new 0x38 and
// constructor RVA 0x00484DF4; buildFieldParse RVA 0x0048490E adds VA 0x00C4A388 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_modifierName +0x14: INI::parseAsciiStringVectorAppend 0x0002E896
#pragma once
#ifndef BFME2_PASSIVE_AREA_EFFECT_BEHAVIOR_MODULE_DATA_H
#define BFME2_PASSIVE_AREA_EFFECT_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class FXList;

class PassiveAreaEffectBehaviorModuleData : public ModuleData
{
public:
	PassiveAreaEffectBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_effectRadius;                                   // +0x8 EffectRadius
	Real m_healPercentPerSecond;                           // +0xC HealPercentPerSecond; percent
	UnsignedInt m_pingDelay;                               // +0x10 PingDelay; duration, frames
	UnsignedByte m_modifierName[0xC];                      // +0x14 ModifierName; unresolved: INI::parseAsciiStringVectorAppend
	ObjectFilter m_allowFilter;                            // +0x20 AllowFilter
	AsciiString m_upgradeRequired;                         // +0x24 UpgradeRequired
	Bool m_nonStackable;                                   // +0x28 NonStackable
	BitFlags<15> m_antiCategories;                         // +0x2C AntiCategories; BitFlags<15>
	const FXList * m_antiFX;                               // +0x30 AntiFX
	const FXList * m_healFX;                               // +0x34 HealFX
};
// SIZE 0x38
typedef char PassiveAreaEffectBehaviorModuleData_size[(sizeof(PassiveAreaEffectBehaviorModuleData) == 0x38) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_effectRadius[(offsetof(PassiveAreaEffectBehaviorModuleData, m_effectRadius) == 0x8) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_healPercentPerSecond[(offsetof(PassiveAreaEffectBehaviorModuleData, m_healPercentPerSecond) == 0xC) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_pingDelay[(offsetof(PassiveAreaEffectBehaviorModuleData, m_pingDelay) == 0x10) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_modifierName[(offsetof(PassiveAreaEffectBehaviorModuleData, m_modifierName) == 0x14) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_allowFilter[(offsetof(PassiveAreaEffectBehaviorModuleData, m_allowFilter) == 0x20) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_upgradeRequired[(offsetof(PassiveAreaEffectBehaviorModuleData, m_upgradeRequired) == 0x24) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_nonStackable[(offsetof(PassiveAreaEffectBehaviorModuleData, m_nonStackable) == 0x28) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_antiCategories[(offsetof(PassiveAreaEffectBehaviorModuleData, m_antiCategories) == 0x2C) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_antiFX[(offsetof(PassiveAreaEffectBehaviorModuleData, m_antiFX) == 0x30) ? 1 : -1];
typedef char PassiveAreaEffectBehaviorModuleData_off_m_healFX[(offsetof(PassiveAreaEffectBehaviorModuleData, m_healFX) == 0x34) ? 1 : -1];

#endif
