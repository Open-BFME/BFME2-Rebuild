// StealthUpdateModuleData.h: promoted from Draft by f10 (2026-11-17); StealthUpdate rows read it. Generated from retail's
// field-parse table (VA 0x00C18210, 36 entries).
// StealthUpdateModuleData.h: the INI data of StealthUpdate. SIZE 0xC8.
// Target facts: ModuleFactory registers the module with operator new 0xC8 and
// constructor RVA 0x0037588B; buildFieldParse RVA 0x00373F2C adds VA 0x00C18210 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_voiceMoveToStealthyArea +0x80: Rva003393DFParse 0x003393DF (8 bytes: token -> 0x00339235)
//   m_voiceEnterStateMoveToStealthyArea +0x88: Rva003393DFParse 0x003393DF
// The three Eva events are Int: Eva::parseEvaEventID 0x001DEE74 stores TheEva->iniScanEvaEventID(token).
#pragma once
#ifndef BFME2_STEALTH_UPDATE_MODULE_DATA_H
#define BFME2_STEALTH_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/BitFlags.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;
class FXList;

class StealthUpdateModuleData : public ModuleData
{
public:
	StealthUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_stealthDelay;                            // +0x8 StealthDelay; duration, frames
	UnsignedInt m_stealthForbiddenConditions;              // +0xC StealthForbiddenConditions; bit string
	ObjectStatusMaskType m_hintDetectableConditions;       // +0x10 HintDetectableConditions; BitFlags<101>
	Real m_moveThresholdSpeed;                             // +0x20 MoveThresholdSpeed; velocity
	Real m_friendlyOpacityMin;                             // +0x24 FriendlyOpacityMin; percent
	Real m_friendlyOpacityMax;                             // +0x28 FriendlyOpacityMax; percent
	UnsignedInt m_pulseFrequency;                          // +0x2C PulseFrequency; duration, frames
	Bool m_disguisesAsTeam;                                // +0x30 DisguisesAsTeam
	Real m_revealDistanceFromTarget;                       // +0x34 RevealDistanceFromTarget
	Bool m_orderIdleEnemiesToAttackMeUponReveal;           // +0x38 OrderIdleEnemiesToAttackMeUponReveal
	const FXList * m_disguiseRevealFX;                     // +0x3C DisguiseRevealFX
	const FXList * m_disguiseFX;                           // +0x40 DisguiseFX
	const FXList * m_becomeStealthedFX;                    // +0x44 BecomeStealthedFX
	const FXList * m_exitStealthFX;                        // +0x48 ExitStealthFX
	const FXList * m_becomeStealthedOneRingFX;             // +0x4C BecomeStealthedOneRingFX
	const FXList * m_exitStealthOneRingFX;                 // +0x50 ExitStealthOneRingFX
	Bool m_startsActive;                                   // +0x54 StartsActive
	Bool m_innateStealth;                                  // +0x55 InnateStealth
	Bool m_detectedByFriendliesOnly;                       // +0x56 DetectedByFriendliesOnly
	UnsignedInt m_disguiseTransitionTime;                  // +0x58 DisguiseTransitionTime; duration, frames
	UnsignedInt m_disguiseRevealTransitionTime;            // +0x5C DisguiseRevealTransitionTime; duration, frames
	Real m_detectedByAnyoneRange;                          // +0x60 DetectedByAnyoneRange
	WeaponSetFlags m_revealWeaponSets;                     // +0x64 RevealWeaponSets; BitFlags<104>
	_STL::vector<AsciiString> m_removeTerrainRestrictionOnUpgrade;// +0x74 RemoveTerrainRestrictionOnUpgrade
	UnsignedByte m_voiceMoveToStealthyArea[0x8];           // +0x80 VoiceMoveToStealthyArea; unresolved: Rva003393DFParse
	UnsignedByte m_voiceEnterStateMoveToStealthyArea[0x8]; // +0x88 VoiceEnterStateMoveToStealthyArea; unresolved: Rva003393DFParse
	UnsignedInt m_oneRingDelayOn;                          // +0x90 OneRingDelayOn; duration, frames
	UnsignedInt m_oneRingDelayOff;                         // +0x94 OneRingDelayOff; duration, frames
	UnsignedInt m_ringAnimTimeOn;                          // +0x98 RingAnimTimeOn; duration, frames
	UnsignedInt m_ringAnimTimeOff;                         // +0x9C RingAnimTimeOff; duration, frames
	UnsignedInt m_ringDelayAfterRemoving;                  // +0xA0 RingDelayAfterRemoving; duration, frames
	Int m_evaEventDetectedEnemy;                         // +0xA4 EvaEventDetectedEnemy; Eva::parseEvaEventID
	Int m_evaEventDetectedAlly;                         // +0xA8 EvaEventDetectedAlly; Eva::parseEvaEventID
	Int m_evaEventDetectedOwner;                         // +0xAC EvaEventDetectedOwner; Eva::parseEvaEventID
	_STL::vector<AsciiString> m_requiredUpgradeNames;      // +0xB0 RequiredUpgradeNames
	_STL::vector<AsciiString> m_forbiddenUpgradeNames;     // +0xBC ForbiddenUpgradeNames
};
// SIZE 0xC8
typedef char StealthUpdateModuleData_size[(sizeof(StealthUpdateModuleData) == 0xC8) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_stealthDelay[(offsetof(StealthUpdateModuleData, m_stealthDelay) == 0x8) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_stealthForbiddenConditions[(offsetof(StealthUpdateModuleData, m_stealthForbiddenConditions) == 0xC) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_hintDetectableConditions[(offsetof(StealthUpdateModuleData, m_hintDetectableConditions) == 0x10) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_moveThresholdSpeed[(offsetof(StealthUpdateModuleData, m_moveThresholdSpeed) == 0x20) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_friendlyOpacityMin[(offsetof(StealthUpdateModuleData, m_friendlyOpacityMin) == 0x24) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_friendlyOpacityMax[(offsetof(StealthUpdateModuleData, m_friendlyOpacityMax) == 0x28) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_pulseFrequency[(offsetof(StealthUpdateModuleData, m_pulseFrequency) == 0x2C) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_disguisesAsTeam[(offsetof(StealthUpdateModuleData, m_disguisesAsTeam) == 0x30) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_revealDistanceFromTarget[(offsetof(StealthUpdateModuleData, m_revealDistanceFromTarget) == 0x34) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_orderIdleEnemiesToAttackMeUponReveal[(offsetof(StealthUpdateModuleData, m_orderIdleEnemiesToAttackMeUponReveal) == 0x38) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_disguiseRevealFX[(offsetof(StealthUpdateModuleData, m_disguiseRevealFX) == 0x3C) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_disguiseFX[(offsetof(StealthUpdateModuleData, m_disguiseFX) == 0x40) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_becomeStealthedFX[(offsetof(StealthUpdateModuleData, m_becomeStealthedFX) == 0x44) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_exitStealthFX[(offsetof(StealthUpdateModuleData, m_exitStealthFX) == 0x48) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_becomeStealthedOneRingFX[(offsetof(StealthUpdateModuleData, m_becomeStealthedOneRingFX) == 0x4C) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_exitStealthOneRingFX[(offsetof(StealthUpdateModuleData, m_exitStealthOneRingFX) == 0x50) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_startsActive[(offsetof(StealthUpdateModuleData, m_startsActive) == 0x54) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_innateStealth[(offsetof(StealthUpdateModuleData, m_innateStealth) == 0x55) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_detectedByFriendliesOnly[(offsetof(StealthUpdateModuleData, m_detectedByFriendliesOnly) == 0x56) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_disguiseTransitionTime[(offsetof(StealthUpdateModuleData, m_disguiseTransitionTime) == 0x58) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_disguiseRevealTransitionTime[(offsetof(StealthUpdateModuleData, m_disguiseRevealTransitionTime) == 0x5C) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_detectedByAnyoneRange[(offsetof(StealthUpdateModuleData, m_detectedByAnyoneRange) == 0x60) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_revealWeaponSets[(offsetof(StealthUpdateModuleData, m_revealWeaponSets) == 0x64) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_removeTerrainRestrictionOnUpgrade[(offsetof(StealthUpdateModuleData, m_removeTerrainRestrictionOnUpgrade) == 0x74) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_voiceMoveToStealthyArea[(offsetof(StealthUpdateModuleData, m_voiceMoveToStealthyArea) == 0x80) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_voiceEnterStateMoveToStealthyArea[(offsetof(StealthUpdateModuleData, m_voiceEnterStateMoveToStealthyArea) == 0x88) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_oneRingDelayOn[(offsetof(StealthUpdateModuleData, m_oneRingDelayOn) == 0x90) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_oneRingDelayOff[(offsetof(StealthUpdateModuleData, m_oneRingDelayOff) == 0x94) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_ringAnimTimeOn[(offsetof(StealthUpdateModuleData, m_ringAnimTimeOn) == 0x98) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_ringAnimTimeOff[(offsetof(StealthUpdateModuleData, m_ringAnimTimeOff) == 0x9C) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_ringDelayAfterRemoving[(offsetof(StealthUpdateModuleData, m_ringDelayAfterRemoving) == 0xA0) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_evaEventDetectedEnemy[(offsetof(StealthUpdateModuleData, m_evaEventDetectedEnemy) == 0xA4) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_evaEventDetectedAlly[(offsetof(StealthUpdateModuleData, m_evaEventDetectedAlly) == 0xA8) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_evaEventDetectedOwner[(offsetof(StealthUpdateModuleData, m_evaEventDetectedOwner) == 0xAC) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_requiredUpgradeNames[(offsetof(StealthUpdateModuleData, m_requiredUpgradeNames) == 0xB0) ? 1 : -1];
typedef char StealthUpdateModuleData_off_m_forbiddenUpgradeNames[(offsetof(StealthUpdateModuleData, m_forbiddenUpgradeNames) == 0xBC) ? 1 : -1];

#endif
