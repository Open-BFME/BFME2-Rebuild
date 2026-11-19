// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C550C8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// GloriousChargeUpdateModuleData.h: the INI data of GloriousChargeUpdate. SIZE 0xD4.
// Target facts: ModuleFactory registers the module with operator new 0xD4 and
// constructor RVA 0x004AD4F2; buildFieldParse RVA 0x004AD51D calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C550C8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_GLORIOUS_CHARGE_UPDATE_MODULE_DATA_H
#define BFME2_GLORIOUS_CHARGE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char GloriousChargeUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class GloriousChargeUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	GloriousChargeUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_bonusRadius;                                    // +0xC8 BonusRadius
	UnsignedInt m_speechDuration;                          // +0xCC SpeechDuration; duration, frames
	UnsignedInt m_updateInterval;                          // +0xD0 UpdateInterval; duration, frames
};
// SIZE 0xD4
typedef char GloriousChargeUpdateModuleData_size[(sizeof(GloriousChargeUpdateModuleData) == 0xD4) ? 1 : -1];
typedef char GloriousChargeUpdateModuleData_off_m_bonusRadius[(offsetof(GloriousChargeUpdateModuleData, m_bonusRadius) == 0xC8) ? 1 : -1];
typedef char GloriousChargeUpdateModuleData_off_m_speechDuration[(offsetof(GloriousChargeUpdateModuleData, m_speechDuration) == 0xCC) ? 1 : -1];
typedef char GloriousChargeUpdateModuleData_off_m_updateInterval[(offsetof(GloriousChargeUpdateModuleData, m_updateInterval) == 0xD0) ? 1 : -1];

#endif
