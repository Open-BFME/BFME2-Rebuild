// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C4FF58).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_bombardmentPlanUnpackSoundName, m_bombardmentPlanPackSoundName, m_bombardmentAnnouncementName, m_searchAndDestroyPlanUnpackSoundName, m_searchAndDestroyPlanIdleLoopSoundName, m_searchAndDestroyPlanPackSoundName, m_searchAndDestroyAnnouncementName, m_holdTheLinePlanUnpackSoundName, m_holdTheLinePlanPackSoundName, m_holdTheLineAnnouncementName.
// BattlePlanUpdateModuleData.h: the INI data of BattlePlanUpdate. SIZE 0xA8.
// Target facts: ModuleFactory registers the module with operator new 0xA8 and
// constructor RVA 0x0049780A; buildFieldParse RVA 0x0049861A adds VA 0x00C4FF58 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_bombardmentPlanUnpackSoundName +0x1C: INI::parseAudioEventRTS 0x00339900
//   m_bombardmentPlanPackSoundName +0x20: INI::parseAudioEventRTS 0x00339900
//   m_bombardmentAnnouncementName +0x28: INI::parseAudioEventRTS 0x00339900
//   m_searchAndDestroyPlanUnpackSoundName +0x2C: INI::parseAudioEventRTS 0x00339900
//   m_searchAndDestroyPlanIdleLoopSoundName +0x30: INI::parseAudioEventRTS 0x00339900
//   m_searchAndDestroyPlanPackSoundName +0x34: INI::parseAudioEventRTS 0x00339900
//   m_searchAndDestroyAnnouncementName +0x3C: INI::parseAudioEventRTS 0x00339900
//   m_holdTheLinePlanUnpackSoundName +0x40: INI::parseAudioEventRTS 0x00339900
//   m_holdTheLinePlanPackSoundName +0x44: INI::parseAudioEventRTS 0x00339900
//   m_holdTheLineAnnouncementName +0x4C: INI::parseAudioEventRTS 0x00339900
#pragma once
#ifndef BFME2_BATTLE_PLAN_UPDATE_MODULE_DATA_H
#define BFME2_BATTLE_PLAN_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"
#include "Common/AudioEventRef.h"

class INI;
class MultiIniFieldParse;
class SpecialPowerTemplate;

class BattlePlanUpdateModuleData : public ModuleData
{
public:
	BattlePlanUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const SpecialPowerTemplate * m_specialPowerTemplate;   // +0x8 SpecialPowerTemplate
	UnsignedInt m_bombardmentPlanAnimationTime;            // +0xC BombardmentPlanAnimationTime; duration, frames
	UnsignedInt m_holdTheLinePlanAnimationTime;            // +0x10 HoldTheLinePlanAnimationTime; duration, frames
	UnsignedInt m_searchAndDestroyPlanAnimationTime;       // +0x14 SearchAndDestroyPlanAnimationTime; duration, frames
	UnsignedInt m_transitionIdleTime;                      // +0x18 TransitionIdleTime; duration, frames
	PooledAudioEventRTSRef m_bombardmentPlanUnpackSoundName; // +0x1C BombardmentPlanUnpackSoundName; INI::parseAudioEventRTS (4-byte refcounted ref)
	PooledAudioEventRTSRef m_bombardmentPlanPackSoundName; // +0x20 BombardmentPlanPackSoundName; INI::parseAudioEventRTS (4-byte refcounted ref)
	AsciiString m_bombardmentMessageLabel;                 // +0x24 BombardmentMessageLabel
	PooledAudioEventRTSRef m_bombardmentAnnouncementName; // +0x28 BombardmentAnnouncementName; INI::parseAudioEventRTS (4-byte refcounted ref)
	PooledAudioEventRTSRef m_searchAndDestroyPlanUnpackSoundName; // +0x2C SearchAndDestroyPlanUnpackSoundName; INI::parseAudioEventRTS (4-byte refcounted ref)
	PooledAudioEventRTSRef m_searchAndDestroyPlanIdleLoopSoundName; // +0x30 SearchAndDestroyPlanIdleLoopSoundName; INI::parseAudioEventRTS (4-byte refcounted ref)
	PooledAudioEventRTSRef m_searchAndDestroyPlanPackSoundName; // +0x34 SearchAndDestroyPlanPackSoundName; INI::parseAudioEventRTS (4-byte refcounted ref)
	AsciiString m_searchAndDestroyMessageLabel;            // +0x38 SearchAndDestroyMessageLabel
	PooledAudioEventRTSRef m_searchAndDestroyAnnouncementName; // +0x3C SearchAndDestroyAnnouncementName; INI::parseAudioEventRTS (4-byte refcounted ref)
	PooledAudioEventRTSRef m_holdTheLinePlanUnpackSoundName; // +0x40 HoldTheLinePlanUnpackSoundName; INI::parseAudioEventRTS (4-byte refcounted ref)
	PooledAudioEventRTSRef m_holdTheLinePlanPackSoundName; // +0x44 HoldTheLinePlanPackSoundName; INI::parseAudioEventRTS (4-byte refcounted ref)
	AsciiString m_holdTheLineMessageLabel;                 // +0x48 HoldTheLineMessageLabel
	PooledAudioEventRTSRef m_holdTheLineAnnouncementName; // +0x4C HoldTheLineAnnouncementName; INI::parseAudioEventRTS (4-byte refcounted ref)
	UnsignedInt m_battlePlanChangeParalyzeTime;            // +0x50 BattlePlanChangeParalyzeTime; duration, frames
	KindOfMaskType m_validMemberKindOf;                    // +0x54 ValidMemberKindOf; BitFlags<218>
	KindOfMaskType m_invalidMemberKindOf;                  // +0x70 InvalidMemberKindOf; BitFlags<218>
	Real m_holdTheLinePlanArmorDamageScalar;               // +0x8C HoldTheLinePlanArmorDamageScalar
	Real m_searchAndDestroyPlanSightRangeScalar;           // +0x90 SearchAndDestroyPlanSightRangeScalar
	Real m_strategyCenterSearchAndDestroySightRangeScalar; // +0x94 StrategyCenterSearchAndDestroySightRangeScalar
	Bool m_strategyCenterSearchAndDestroyDetectsStealth;   // +0x98 StrategyCenterSearchAndDestroyDetectsStealth
	Real m_strategyCenterHoldTheLineMaxHealthScalar;       // +0x9C StrategyCenterHoldTheLineMaxHealthScalar
	Int m_strategyCenterHoldTheLineMaxHealthChangeType;    // +0xA0 StrategyCenterHoldTheLineMaxHealthChangeType; index into a name list (enum)
	AsciiString m_visionObjectName;                        // +0xA4 VisionObjectName
};
// SIZE 0xA8
typedef char BattlePlanUpdateModuleData_size[(sizeof(BattlePlanUpdateModuleData) == 0xA8) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_specialPowerTemplate[(offsetof(BattlePlanUpdateModuleData, m_specialPowerTemplate) == 0x8) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_bombardmentPlanAnimationTime[(offsetof(BattlePlanUpdateModuleData, m_bombardmentPlanAnimationTime) == 0xC) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_holdTheLinePlanAnimationTime[(offsetof(BattlePlanUpdateModuleData, m_holdTheLinePlanAnimationTime) == 0x10) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_searchAndDestroyPlanAnimationTime[(offsetof(BattlePlanUpdateModuleData, m_searchAndDestroyPlanAnimationTime) == 0x14) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_transitionIdleTime[(offsetof(BattlePlanUpdateModuleData, m_transitionIdleTime) == 0x18) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_bombardmentPlanUnpackSoundName[(offsetof(BattlePlanUpdateModuleData, m_bombardmentPlanUnpackSoundName) == 0x1C) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_bombardmentPlanPackSoundName[(offsetof(BattlePlanUpdateModuleData, m_bombardmentPlanPackSoundName) == 0x20) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_bombardmentMessageLabel[(offsetof(BattlePlanUpdateModuleData, m_bombardmentMessageLabel) == 0x24) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_bombardmentAnnouncementName[(offsetof(BattlePlanUpdateModuleData, m_bombardmentAnnouncementName) == 0x28) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_searchAndDestroyPlanUnpackSoundName[(offsetof(BattlePlanUpdateModuleData, m_searchAndDestroyPlanUnpackSoundName) == 0x2C) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_searchAndDestroyPlanIdleLoopSoundName[(offsetof(BattlePlanUpdateModuleData, m_searchAndDestroyPlanIdleLoopSoundName) == 0x30) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_searchAndDestroyPlanPackSoundName[(offsetof(BattlePlanUpdateModuleData, m_searchAndDestroyPlanPackSoundName) == 0x34) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_searchAndDestroyMessageLabel[(offsetof(BattlePlanUpdateModuleData, m_searchAndDestroyMessageLabel) == 0x38) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_searchAndDestroyAnnouncementName[(offsetof(BattlePlanUpdateModuleData, m_searchAndDestroyAnnouncementName) == 0x3C) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_holdTheLinePlanUnpackSoundName[(offsetof(BattlePlanUpdateModuleData, m_holdTheLinePlanUnpackSoundName) == 0x40) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_holdTheLinePlanPackSoundName[(offsetof(BattlePlanUpdateModuleData, m_holdTheLinePlanPackSoundName) == 0x44) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_holdTheLineMessageLabel[(offsetof(BattlePlanUpdateModuleData, m_holdTheLineMessageLabel) == 0x48) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_holdTheLineAnnouncementName[(offsetof(BattlePlanUpdateModuleData, m_holdTheLineAnnouncementName) == 0x4C) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_battlePlanChangeParalyzeTime[(offsetof(BattlePlanUpdateModuleData, m_battlePlanChangeParalyzeTime) == 0x50) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_validMemberKindOf[(offsetof(BattlePlanUpdateModuleData, m_validMemberKindOf) == 0x54) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_invalidMemberKindOf[(offsetof(BattlePlanUpdateModuleData, m_invalidMemberKindOf) == 0x70) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_holdTheLinePlanArmorDamageScalar[(offsetof(BattlePlanUpdateModuleData, m_holdTheLinePlanArmorDamageScalar) == 0x8C) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_searchAndDestroyPlanSightRangeScalar[(offsetof(BattlePlanUpdateModuleData, m_searchAndDestroyPlanSightRangeScalar) == 0x90) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_strategyCenterSearchAndDestroySightRangeScalar[(offsetof(BattlePlanUpdateModuleData, m_strategyCenterSearchAndDestroySightRangeScalar) == 0x94) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_strategyCenterSearchAndDestroyDetectsStealth[(offsetof(BattlePlanUpdateModuleData, m_strategyCenterSearchAndDestroyDetectsStealth) == 0x98) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_strategyCenterHoldTheLineMaxHealthScalar[(offsetof(BattlePlanUpdateModuleData, m_strategyCenterHoldTheLineMaxHealthScalar) == 0x9C) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_strategyCenterHoldTheLineMaxHealthChangeType[(offsetof(BattlePlanUpdateModuleData, m_strategyCenterHoldTheLineMaxHealthChangeType) == 0xA0) ? 1 : -1];
typedef char BattlePlanUpdateModuleData_off_m_visionObjectName[(offsetof(BattlePlanUpdateModuleData, m_visionObjectName) == 0xA4) ? 1 : -1];

#endif
