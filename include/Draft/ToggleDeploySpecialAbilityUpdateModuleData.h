// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BEF820).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_soundDeploy, m_soundUndeploy.
// ToggleDeploySpecialAbilityUpdateModuleData.h: the INI data of ToggleDeploySpecialAbilityUpdate. SIZE 0xD0.
// Target facts: ModuleFactory registers the module with operator new 0xD0 and
// constructor RVA 0x004AE547; buildFieldParse RVA 0x0024F7C3 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEF820 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_soundDeploy +0xC8: INI::parseAudioEventRTS 0x00339900
//   m_soundUndeploy +0xCC: INI::parseAudioEventRTS 0x00339900
#pragma once
#ifndef BFME2_TOGGLE_DEPLOY_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_TOGGLE_DEPLOY_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "Common/OpaqueRefElement4.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char ToggleDeploySpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class ToggleDeploySpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	ToggleDeploySpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	OpaqueRefElement4 m_soundDeploy;                      // +0xC8 SoundDeploy (turnOn 0x004AE6D7 builds an AudioEventRTS from it)
	OpaqueRefElement4 m_soundUndeploy;                    // +0xCC SoundUndeploy (turnOff 0x004AE5B8 builds an AudioEventRTS from it)
};
// SIZE 0xD0
typedef char ToggleDeploySpecialAbilityUpdateModuleData_size[(sizeof(ToggleDeploySpecialAbilityUpdateModuleData) == 0xD0) ? 1 : -1];
typedef char ToggleDeploySpecialAbilityUpdateModuleData_off_m_soundDeploy[(offsetof(ToggleDeploySpecialAbilityUpdateModuleData, m_soundDeploy) == 0xC8) ? 1 : -1];
typedef char ToggleDeploySpecialAbilityUpdateModuleData_off_m_soundUndeploy[(offsetof(ToggleDeploySpecialAbilityUpdateModuleData, m_soundUndeploy) == 0xCC) ? 1 : -1];

#endif
