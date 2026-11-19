// SpecialAbilityUpdateModuleData.h. SIZE 0xC8 (ModuleFactory operator new).
// Names of the former fieldNN members are the INI tokens of retail's table at
// VA 0x00C3F7F8 (buildFieldParse RVA 0x0044ED95); offsets unchanged (h05).
// +0x18 and +0x24 are INI::iniParseAnimAndDuration stores (CustomAnimAndDuration).
#pragma once

#ifndef BFME2_SPECIALABILITYUPDATEMODULEDATA_H
#define BFME2_SPECIALABILITYUPDATEMODULEDATA_H

#include "Common/AsciiString.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/CustomAnimAndDuration.h"
#include "Common/OpaqueRefElement4.h"

class INI;
class ParticleSystemTemplate;
class SpecialPowerTemplate;

class SpecialAbilityUpdateModuleData : public ModuleData
{
public:
    SpecialAbilityUpdateModuleData();
    virtual ~SpecialAbilityUpdateModuleData();		// 0x0044ECCE (an emitted row, no annotation): clears the template and particle pointers, then the five sound handles and four strings release
    static void buildFieldParse(MultiIniFieldParse &);
    static ModuleData *friend_newModuleData(INI *);

    OpaqueRefElement4 m_packSound;                         // +0x08
    OpaqueRefElement4 m_unpackSound;                       // +0x0C
    OpaqueRefElement4 m_prepSoundLoop;                     // +0x10
    OpaqueRefElement4 m_triggerSound;                      // +0x14
    CustomAnimAndDuration m_customAnimAndDuration;       // +0x18 CustomAnimAndDuration
    CustomAnimAndDuration m_grabPassengerAnimAndDuration; // +0x24 GrabPassengerAnimAndDuration
    float m_grabPassengerHealGainPercent;                // +0x30 GrabPassengerHealGainPercent
    OpaqueRefElement4 m_activeLoopSound;                 // +0x34 ActiveLoopSound
    const SpecialPowerTemplate *m_specialPowerTemplate;   // +0x38
    const ParticleSystemTemplate *m_disableFXParticleSystem; // +0x3C
    AsciiString m_specialObjectName;                     // +0x40
    AsciiString m_specialObjectAttachToBoneName;           // +0x44
    AsciiString m_triggerAttributeModifier;              // +0x48 TriggerAttributeModifier
    float m_startAbilityRange;                           // +0x4C
    float m_abilityAbortRange;                           // +0x50
    float m_packUnpackVariationFactor;                    // +0x54
    float m_fleeRangeAfterCompletion;                     // +0x58
    int m_effectValue;                                   // +0x5C
    float m_effectRange;                                 // +0x60 EffectRange
    int m_awardXPForTriggering;                           // +0x64
    int m_skillPointsForTriggering;                       // +0x68
    int m_unpackingVariation;                            // +0x6C UnpackingVariation
    int m_persistentCount;                               // +0x70 PersistentCount
    unsigned int m_preparationFrames;                     // +0x74
    unsigned int m_persistentPrepFrames;                  // +0x78
    unsigned int m_effectDuration;                       // +0x7C
    unsigned int m_maxSpecialObjects;                     // +0x80
    unsigned int m_packTime;                              // +0x84
    unsigned int m_unpackTime;                            // +0x88
    unsigned int m_preTriggerUnstealthFrames;             // +0x8C
    unsigned int m_paralyzeDurationWhenCompleted;        // +0x90 ParalyzeDurationWhenCompleted
    unsigned int m_paralyzeDurationWhenAborted;          // +0x94 ParalyzeDurationWhenAborted
    unsigned int m_attributeModifierDuration;            // +0x98 AttributeModifierDuration
    unsigned int m_freezeAfterTriggerDuration;           // +0x9C FreezeAfterTriggerDuration
    unsigned int m_requiredConditions;                   // +0xA0 RequiredConditions
    unsigned int m_rejectedConditions;                   // +0xA4 RejectedConditions
    bool m_skipPackingWithNoTarget;                       // +0xA8
    bool m_specialObjectsPersistent;                     // +0xA9
    bool m_uniqueSpecialObjectTargets;                    // +0xAA
    bool m_specialObjectsPersistWhenOwnerDies;            // +0xAB
    bool m_flipObjectAfterPacking;                       // +0xAC
    bool m_flipObjectAfterUnpacking;                      // +0xAD
    bool m_alwaysValidateSpecialObjects;                  // +0xAE
    bool m_doCaptureFX;                                  // +0xAF
    bool m_loseStealthOnTrigger;                          // +0xB0
    bool m_approachRequiresLOS;                           // +0xB1
    bool m_chargeAttackSpeedBoost;                       // +0xB2 ChargeAttackSpeedBoost
    bool m_killAttributeModifierOnExit;                  // +0xB3 KillAttributeModifierOnExit
    bool m_killAttributeModifierOnRejected;              // +0xB4 KillAttributeModifierOnRejected
    bool m_mustFinishAbility;                            // +0xB5 MustFinishAbility
    bool m_disableWhenWearingTheRing;                    // +0xB6 DisableWhenWearingTheRing
    bool m_instant;                                      // +0xB7 Instant
    bool m_needCollisionBeforeTrigger;                   // +0xB8 NeedCollisionBeforeTrigger
    AsciiString m_contactPointOverride;                  // +0xBC ContactPointOverride
    AsciiString m_chainedButton;                         // +0xC0 ChainedButton
    bool m_suppressForHordes;                            // +0xC4 SuppressForHordes
    bool m_approachUntilMembersInRange;                  // +0xC5 ApproachUntilMembersInRange
    bool m_ignoreFacingCheck;                            // +0xC6 IgnoreFacingCheck
};
// SIZE 0xC8
typedef char SpecialAbilityUpdateModuleData_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

#endif
