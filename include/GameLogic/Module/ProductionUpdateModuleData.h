// ProductionUpdateModuleData.h: the INI data of ProductionUpdate. SIZE 0x50.
// Target facts: ModuleFactory registers the module with operator new 0x50 and
// constructor RVA 0x0049F7DB; buildFieldParse RVA 0x0049FE6A adds VA 0x00C517F0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// INI::parseReal writes a Real and parseDurationUnsignedInt writes a frame count.
// Members use the INI token names. Byte gaps have no corresponding field entry.
// Unresolved (byte storage up to the next member, named after the token):
//   m_quantityModifier +0x1C: Rva0049FDF9Parse 0x0049FDF9
//   m_speedBonusAudioLoop +0x48: INI::parseAudioEventRTS 0x00339900
#pragma once
#ifndef BFME2_PRODUCTION_UPDATE_MODULE_DATA_H
#define BFME2_PRODUCTION_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/BitFlags.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class ProductionUpdateModuleData : public ModuleData
{
public:
	ProductionUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_numDoorAnimations;                               // +0x8 NumDoorAnimations
	UnsignedInt m_doorOpeningTime;                         // +0xC DoorOpeningTime; duration, frames
	UnsignedInt m_doorWaitOpenTime;                        // +0x10 DoorWaitOpenTime; duration, frames
	UnsignedInt m_doorCloseTime;                           // +0x14 DoorCloseTime; duration, frames
	UnsignedInt m_constructionCompleteDuration;            // +0x18 ConstructionCompleteDuration; duration, frames
	UnsignedByte m_quantityModifier[0xC];                  // +0x1C QuantityModifier; unresolved: Rva0049FDF9Parse
	UnsignedInt m_maxQueueEntries;                         // +0x28 MaxQueueEntries
	BitFlags<11> m_disabledTypesToProcess;	// +0x2C. UpdateModuleInterface slot 1 returns this mask by value.
	Bool m_giveNoXP;                                       // +0x30 GiveNoXP
	UnsignedInt m_unitInvulnerableTime;                    // +0x34 UnitInvulnerableTime; duration, frames
	UnsignedInt m_specialPrepModelconditionTime;           // +0x38 SpecialPrepModelconditionTime; duration, frames
	UnsignedByte m_unk0x3C[0x4];
	Bool m_veteranUnitsFromVeteranFactory;                 // +0x40 VeteranUnitsFromVeteranFactory
	Bool m_setBonusModelConditionOnSpeedBonus;             // +0x41 SetBonusModelConditionOnSpeedBonus
	AsciiString m_bonusForType;                            // +0x44 BonusForType
	UnsignedByte m_speedBonusAudioLoop[0x4];               // +0x48 SpeedBonusAudioLoop; unresolved: INI::parseAudioEventRTS
	Bool m_secondaryQueue;                                 // +0x4C SecondaryQueue
};
// SIZE 0x50
typedef char ProductionUpdateModuleData_size[(sizeof(ProductionUpdateModuleData) == 0x50) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_numDoorAnimations[(offsetof(ProductionUpdateModuleData, m_numDoorAnimations) == 0x8) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_doorOpeningTime[(offsetof(ProductionUpdateModuleData, m_doorOpeningTime) == 0xC) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_doorWaitOpenTime[(offsetof(ProductionUpdateModuleData, m_doorWaitOpenTime) == 0x10) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_doorCloseTime[(offsetof(ProductionUpdateModuleData, m_doorCloseTime) == 0x14) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_constructionCompleteDuration[(offsetof(ProductionUpdateModuleData, m_constructionCompleteDuration) == 0x18) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_quantityModifier[(offsetof(ProductionUpdateModuleData, m_quantityModifier) == 0x1C) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_maxQueueEntries[(offsetof(ProductionUpdateModuleData, m_maxQueueEntries) == 0x28) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_disabledTypesToProcess[(offsetof(ProductionUpdateModuleData, m_disabledTypesToProcess) == 0x2C) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_giveNoXP[(offsetof(ProductionUpdateModuleData, m_giveNoXP) == 0x30) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_unitInvulnerableTime[(offsetof(ProductionUpdateModuleData, m_unitInvulnerableTime) == 0x34) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_specialPrepModelconditionTime[(offsetof(ProductionUpdateModuleData, m_specialPrepModelconditionTime) == 0x38) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_veteranUnitsFromVeteranFactory[(offsetof(ProductionUpdateModuleData, m_veteranUnitsFromVeteranFactory) == 0x40) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_setBonusModelConditionOnSpeedBonus[(offsetof(ProductionUpdateModuleData, m_setBonusModelConditionOnSpeedBonus) == 0x41) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_bonusForType[(offsetof(ProductionUpdateModuleData, m_bonusForType) == 0x44) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_speedBonusAudioLoop[(offsetof(ProductionUpdateModuleData, m_speedBonusAudioLoop) == 0x48) ? 1 : -1];
typedef char ProductionUpdateModuleData_off_m_secondaryQueue[(offsetof(ProductionUpdateModuleData, m_secondaryQueue) == 0x4C) ? 1 : -1];

#endif
