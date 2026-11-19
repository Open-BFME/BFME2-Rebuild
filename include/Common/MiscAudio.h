#pragma once
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// MiscAudio: the INI MiscAudio block, 61 audio events of four bytes each (SIZE 0xF4, the extent
// of its constructor 0x001ED70C). Offsets and order come from the field table (VA 0x00BDF2D0);
// member names are Zero Hour's where the INI token is the same, the token otherwise. The
// audio-event store type INI::parseAudioEventRTS writes is not resolved fleet-wide, so each
// field is a four-byte owning handle (OpaqueRefElement4: AudioEventRTS's constructor 0x002DA461 takes it; f10).
#include <stddef.h>
#include "Common/OpaqueRefElement4.h"
struct FieldParse;
struct MiscAudio {
    OpaqueRefElement4 m_radarHarvesterUnderAttackSound;        // +0x00 RadarNotifyHarvesterUnderAttackSound
    OpaqueRefElement4 m_radarStructureUnderAttackSound;        // +0x04 RadarNotifyStructureUnderAttackSound
    OpaqueRefElement4 m_radarInfiltrationSound;                // +0x08 RadarNotifyInfiltrationSound
    OpaqueRefElement4 m_radarOnlineSound;                      // +0x0C RadarNotifyOnlineSound
    OpaqueRefElement4 m_radarOfflineSound;                     // +0x10 RadarNotifyOfflineSound
    OpaqueRefElement4 m_genericRadarEvent;                     // +0x14 GenericRadarEvent
    OpaqueRefElement4 m_beaconPlacedSound;                     // +0x18 BeaconPlacedSound
    OpaqueRefElement4 m_beaconPlacementFailed;                 // +0x1C BeaconPlacementFailed
    OpaqueRefElement4 m_defectorTimerTickSound;                // +0x20 DefectorTimerTickSound
    OpaqueRefElement4 m_defectorTimerDingSound;                // +0x24 DefectorTimerDingSound
    OpaqueRefElement4 m_allCheerSound;                         // +0x28 AllCheerSound
    OpaqueRefElement4 m_noCanDoSound;                          // +0x2C NoCanDoSound
    OpaqueRefElement4 m_stealthDiscoveredSound;                // +0x30 StealthDiscoveredSound
    OpaqueRefElement4 m_stealthNeutralizedSound;               // +0x34 StealthNeutralizedSound
    OpaqueRefElement4 m_moneyDepositSound;                     // +0x38 MoneyDepositSound
    OpaqueRefElement4 m_moneyWithdrawSound;                    // +0x3C MoneyWithdrawSound
    OpaqueRefElement4 m_buildingDisabled;                      // +0x40 BuildingDisabled
    OpaqueRefElement4 m_buildingReenabled;                     // +0x44 BuildingReenabled
    OpaqueRefElement4 m_vehicleDisabled;                       // +0x48 VehicleDisabled
    OpaqueRefElement4 m_vehicleReenabled;                      // +0x4C VehicleReenabled
    OpaqueRefElement4 m_splatterVehiclePilotsBrain;            // +0x50 SplatterVehiclePilotsBrain
    OpaqueRefElement4 m_crateHeal;                             // +0x54 CrateHeal
    OpaqueRefElement4 m_crateShroud;                           // +0x58 CrateShroud
    OpaqueRefElement4 m_crateFreeUnit;                         // +0x5C CrateFreeUnit
    OpaqueRefElement4 m_crateMoney;                            // +0x60 CrateMoney
    OpaqueRefElement4 m_unitPromoted;                          // +0x64 UnitPromoted
    OpaqueRefElement4 m_repairSparks;                          // +0x68 RepairSparks
    OpaqueRefElement4 m_enterCloseCombat;                      // +0x6C EnterCloseCombat
    OpaqueRefElement4 m_exitCloseCombat;                       // +0x70 ExitCloseCombat
    OpaqueRefElement4 m_incomingChatNotification;              // +0x74 IncomingChatNotification
    OpaqueRefElement4 m_privateMessageNotification;            // +0x78 PrivateMessageNotification
    OpaqueRefElement4 m_buddyMessageNotification;              // +0x7C BuddyMessageNotification
    OpaqueRefElement4 m_enabledHotKeyPressed;                  // +0x80 EnabledHotKeyPressed
    OpaqueRefElement4 m_disabledHotKeyPressed;                 // +0x84 DisabledHotKeyPressed
    OpaqueRefElement4 m_disabledButtonClicked;                 // +0x88 DisabledButtonClicked
    OpaqueRefElement4 m_lowLODShellMusic;                      // +0x8C LowLODShellMusic
    OpaqueRefElement4 m_highLODShellMusic;                     // +0x90 HighLODShellMusic
    OpaqueRefElement4 m_scoreScreenMusic;                      // +0x94 ScoreScreenMusic
    OpaqueRefElement4 m_shellMapLoadMusic;                     // +0x98 ShellMapLoadMusic
    OpaqueRefElement4 m_fullScreenSubMenuMusic;                // +0x9C FullScreenSubMenuMusic
    OpaqueRefElement4 m_saveFileLoadMusic;                     // +0xA0 SaveFileLoadMusic
    OpaqueRefElement4 m_creditsMusic;                          // +0xA4 CreditsMusic
    OpaqueRefElement4 m_volumeSampleSoundFX;                   // +0xA8 VolumeSampleSoundFX
    OpaqueRefElement4 m_volumeSampleVoice;                     // +0xAC VolumeSampleVoice
    OpaqueRefElement4 m_volumeSampleMusic;                     // +0xB0 VolumeSampleMusic
    OpaqueRefElement4 m_volumeSampleAmbient;                   // +0xB4 VolumeSampleAmbient
    OpaqueRefElement4 m_volumeSampleMovie;                     // +0xB8 VolumeSampleMovie
    OpaqueRefElement4 m_missionBriefingCharacterClick;         // +0xBC MissionBriefingCharacterClick
    OpaqueRefElement4 m_comboBoxClick;                         // +0xC0 ComboBoxClick
    OpaqueRefElement4 m_gameSpyCommunicatorOpen;               // +0xC4 GameSpyCommunicatorOpen
    OpaqueRefElement4 m_rIFThingTemplateReloadedSound;         // +0xC8 RIFThingTemplateReloadedSound
    OpaqueRefElement4 m_rIFObjectsRefreshedSound;              // +0xCC RIFObjectsRefreshedSound
    OpaqueRefElement4 m_fastForwardModeOn;                     // +0xD0 FastForwardModeOn
    OpaqueRefElement4 m_fastForwardModeOff;                    // +0xD4 FastForwardModeOff
    OpaqueRefElement4 m_rallyPointSet;                         // +0xD8 RallyPointSet
    OpaqueRefElement4 m_unableToSetRallyPoint;                 // +0xDC UnableToSetRallyPoint
    OpaqueRefElement4 m_planningModeOrderGiven;                // +0xE0 PlanningModeOrderGiven
    OpaqueRefElement4 m_buildingPlacementSound;                // +0xE4 BuildingPlacementSound
    OpaqueRefElement4 m_badBuildingPlacementSound;             // +0xE8 BadBuildingPlacementSound
    OpaqueRefElement4 m_wallPlacementSound;                    // +0xEC WallPlacementSound
    OpaqueRefElement4 m_targetObjectWithSpecialPowerSound;     // +0xF0 TargetObjectWithSpecialPowerSound

    static const FieldParse m_fieldParseTable[];
};
typedef char MiscAudio_size[(sizeof(MiscAudio) == 0xF4) ? 1 : -1];
