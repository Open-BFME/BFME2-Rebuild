// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C52468).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_invisibilityNugget, m_requiredUpgrades, m_forbiddenUpgrades.
// InvisibilityUpdateModuleData.h: the INI data of InvisibilityUpdate. SIZE 0x1DC.
// Target facts: ModuleFactory registers the module with operator new 0x1DC and
// constructor RVA 0x00255474; buildFieldParse RVA 0x004A381C adds VA 0x00C52468 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_invisibilityNugget +0x8: Rva00438176Parse 0x00438176
//   m_requiredUpgrades +0xC4: Rva0026F28BParse 0x0026F28B
//   m_forbiddenUpgrades +0x144: Rva0026F28BParse 0x0026F28B
#pragma once
#ifndef BFME2_INVISIBILITY_UPDATE_MODULE_DATA_H
#define BFME2_INVISIBILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class InvisibilityUpdateModuleData : public ModuleData
{
public:
	static ModuleData *friend_newModuleData(INI *ini);
	InvisibilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_invisibilityNugget[0xB8];               // +0x8 InvisibilityNugget; unresolved: Rva00438176Parse
	UnsignedInt m_updatePeriod;                            // +0xC0 UpdatePeriod; duration, frames
	UnsignedByte m_requiredUpgrades[0x80];                 // +0xC4 RequiredUpgrades; unresolved: Rva0026F28BParse
	UnsignedByte m_forbiddenUpgrades[0x80];                // +0x144 ForbiddenUpgrades; unresolved: Rva0026F28BParse
	Bool m_broadcast;                                      // +0x1C4 Broadcast
	ObjectFilter m_broadcastObjectFilter;                  // +0x1C8 BroadcastObjectFilter
	Real m_broadcastRange;                                 // +0x1CC BroadcastRange
	Bool m_startsActive;                                   // +0x1D0 StartsActive
	AsciiString m_unitSpecificSoundNameToUseAsVoiceMoveToStealthyArea;// +0x1D4 UnitSpecificSoundNameToUseAsVoiceMoveToStealthyArea
	AsciiString m_unitSpecificSoundNameToUseAsVoiceEnterStateMoveToStealthyArea;// +0x1D8 UnitSpecificSoundNameToUseAsVoiceEnterStateMoveToStealthyArea
};
// SIZE 0x1DC
typedef char InvisibilityUpdateModuleData_size[(sizeof(InvisibilityUpdateModuleData) == 0x1DC) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_invisibilityNugget[(offsetof(InvisibilityUpdateModuleData, m_invisibilityNugget) == 0x8) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_updatePeriod[(offsetof(InvisibilityUpdateModuleData, m_updatePeriod) == 0xC0) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_requiredUpgrades[(offsetof(InvisibilityUpdateModuleData, m_requiredUpgrades) == 0xC4) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_forbiddenUpgrades[(offsetof(InvisibilityUpdateModuleData, m_forbiddenUpgrades) == 0x144) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_broadcast[(offsetof(InvisibilityUpdateModuleData, m_broadcast) == 0x1C4) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_broadcastObjectFilter[(offsetof(InvisibilityUpdateModuleData, m_broadcastObjectFilter) == 0x1C8) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_broadcastRange[(offsetof(InvisibilityUpdateModuleData, m_broadcastRange) == 0x1CC) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_startsActive[(offsetof(InvisibilityUpdateModuleData, m_startsActive) == 0x1D0) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_unitSpecificSoundNameToUseAsVoiceMoveToStealthyArea[(offsetof(InvisibilityUpdateModuleData, m_unitSpecificSoundNameToUseAsVoiceMoveToStealthyArea) == 0x1D4) ? 1 : -1];
typedef char InvisibilityUpdateModuleData_off_m_unitSpecificSoundNameToUseAsVoiceEnterStateMoveToStealthyArea[(offsetof(InvisibilityUpdateModuleData, m_unitSpecificSoundNameToUseAsVoiceEnterStateMoveToStealthyArea) == 0x1D8) ? 1 : -1];

#endif
