// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5BAE8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DelayedDeathBodyModuleData.h: the INI data of DelayedDeathBody. SIZE 0x80.
// Target facts: ModuleFactory registers the module with operator new 0x80 and
// constructor RVA 0x004C180A; buildFieldParse RVA 0x004C159E calls RespawnBodyModuleData::buildFieldParse, then adds VA 0x00C5BAE8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DELAYED_DEATH_BODY_MODULE_DATA_H
#define BFME2_DELAYED_DEATH_BODY_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/RespawnBodyModuleData.h"

class INI;
class MultiIniFieldParse;
class FXList;
class UpgradeTemplate;

// Base RespawnBodyModuleData: the constructor calls its constructor on this. It ends at 0x6C (its own operator-new size);
// this class's members follow it.
typedef char DelayedDeathBodyModuleData_base_size[(sizeof(RespawnBodyModuleData) == 0x6C) ? 1 : -1];

class DelayedDeathBodyModuleData : public RespawnBodyModuleData
{
public:
	DelayedDeathBodyModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_delayedDeathTime;                        // +0x6C DelayedDeathTime; duration, frames
	Bool m_immortalUntilDeathTime;                         // +0x70 ImmortalUntilDeathTime
	const FXList * m_invulnerableFX;                       // +0x74 InvulnerableFX
	Bool m_doHealthCheck;                                  // +0x78 DoHealthCheck
	const UpgradeTemplate * m_delayedDeathPrerequisiteUpgrade;// +0x7C DelayedDeathPrerequisiteUpgrade
};
// SIZE 0x80
typedef char DelayedDeathBodyModuleData_size[(sizeof(DelayedDeathBodyModuleData) == 0x80) ? 1 : -1];
typedef char DelayedDeathBodyModuleData_off_m_delayedDeathTime[(offsetof(DelayedDeathBodyModuleData, m_delayedDeathTime) == 0x6C) ? 1 : -1];
typedef char DelayedDeathBodyModuleData_off_m_immortalUntilDeathTime[(offsetof(DelayedDeathBodyModuleData, m_immortalUntilDeathTime) == 0x70) ? 1 : -1];
typedef char DelayedDeathBodyModuleData_off_m_invulnerableFX[(offsetof(DelayedDeathBodyModuleData, m_invulnerableFX) == 0x74) ? 1 : -1];
typedef char DelayedDeathBodyModuleData_off_m_doHealthCheck[(offsetof(DelayedDeathBodyModuleData, m_doHealthCheck) == 0x78) ? 1 : -1];
typedef char DelayedDeathBodyModuleData_off_m_delayedDeathPrerequisiteUpgrade[(offsetof(DelayedDeathBodyModuleData, m_delayedDeathPrerequisiteUpgrade) == 0x7C) ? 1 : -1];

#endif
