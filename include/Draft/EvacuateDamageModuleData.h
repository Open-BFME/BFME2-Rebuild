// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C59DD0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// EvacuateDamageModuleData.h: the INI data of EvacuateDamage. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x004BAD10; buildFieldParse RVA 0x004BAD64 adds VA 0x00C59DD0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_EVACUATE_DAMAGE_MODULE_DATA_H
#define BFME2_EVACUATE_DAMAGE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class EvacuateDamageModuleData : public ModuleData
{
public:
	EvacuateDamageModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_unk0x8[0x4];
	Int m_damageTypeToTrack;                               // +0xC DamageTypeToTrack; index into a name list (enum)
	Real m_damageToPanicThreshold;                         // +0x10 DamageToPanicThreshold; percent
	UnsignedInt m_trackingTimeSpan;                        // +0x14 TrackingTimeSpan; duration, frames
};
// SIZE 0x18
typedef char EvacuateDamageModuleData_size[(sizeof(EvacuateDamageModuleData) == 0x18) ? 1 : -1];
typedef char EvacuateDamageModuleData_off_m_damageTypeToTrack[(offsetof(EvacuateDamageModuleData, m_damageTypeToTrack) == 0xC) ? 1 : -1];
typedef char EvacuateDamageModuleData_off_m_damageToPanicThreshold[(offsetof(EvacuateDamageModuleData, m_damageToPanicThreshold) == 0x10) ? 1 : -1];
typedef char EvacuateDamageModuleData_off_m_trackingTimeSpan[(offsetof(EvacuateDamageModuleData, m_trackingTimeSpan) == 0x14) ? 1 : -1];

#endif
