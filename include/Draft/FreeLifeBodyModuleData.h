// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5BD38).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_freeLifeAnimAndDuration.
// FreeLifeBodyModuleData.h: the INI data of FreeLifeBody. SIZE 0x88.
// Target facts: ModuleFactory registers the module with operator new 0x88 and
// constructor RVA 0x004C1B3B; buildFieldParse RVA 0x004C184B calls RespawnBodyModuleData::buildFieldParse, then adds VA 0x00C5BD38 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_freeLifeAnimAndDuration +0x78: INI::iniParseAnimAndDuration 0x0044E6CD
#pragma once
#ifndef BFME2_FREE_LIFE_BODY_MODULE_DATA_H
#define BFME2_FREE_LIFE_BODY_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/RespawnBodyModuleData.h"

class INI;
class MultiIniFieldParse;
class UpgradeTemplate;

// Base RespawnBodyModuleData: the constructor calls its constructor on this. It ends at 0x6C (its own operator-new size);
// this class's members follow it.
typedef char FreeLifeBodyModuleData_base_size[(sizeof(RespawnBodyModuleData) == 0x6C) ? 1 : -1];

class FreeLifeBodyModuleData : public RespawnBodyModuleData
{
public:
	FreeLifeBodyModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_freeLifeHealthPercent;                          // +0x6C FreeLifeHealthPercent; percent
	UnsignedInt m_freeLifeTime;                            // +0x70 FreeLifeTime; duration, frames
	Bool m_freeLifeInvincible;                             // +0x74 FreeLifeInvincible
	UnsignedByte m_unk0x75[0x3];
	UnsignedByte m_freeLifeAnimAndDuration[0xC];           // +0x78 FreeLifeAnimAndDuration; unresolved: INI::iniParseAnimAndDuration
	const UpgradeTemplate * m_freeLifePrerequisiteUpgrade; // +0x84 FreeLifePrerequisiteUpgrade
};
// SIZE 0x88
typedef char FreeLifeBodyModuleData_size[(sizeof(FreeLifeBodyModuleData) == 0x88) ? 1 : -1];
typedef char FreeLifeBodyModuleData_off_m_freeLifeHealthPercent[(offsetof(FreeLifeBodyModuleData, m_freeLifeHealthPercent) == 0x6C) ? 1 : -1];
typedef char FreeLifeBodyModuleData_off_m_freeLifeTime[(offsetof(FreeLifeBodyModuleData, m_freeLifeTime) == 0x70) ? 1 : -1];
typedef char FreeLifeBodyModuleData_off_m_freeLifeInvincible[(offsetof(FreeLifeBodyModuleData, m_freeLifeInvincible) == 0x74) ? 1 : -1];
typedef char FreeLifeBodyModuleData_off_m_freeLifeAnimAndDuration[(offsetof(FreeLifeBodyModuleData, m_freeLifeAnimAndDuration) == 0x78) ? 1 : -1];
typedef char FreeLifeBodyModuleData_off_m_freeLifePrerequisiteUpgrade[(offsetof(FreeLifeBodyModuleData, m_freeLifePrerequisiteUpgrade) == 0x84) ? 1 : -1];

#endif
