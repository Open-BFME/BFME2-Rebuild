// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5E298).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// PlayerHealSpecialPowerModuleData.h: the INI data of PlayerHealSpecialPower. SIZE 0xAC.
// Target facts: ModuleFactory registers the module with operator new 0xAC and
// constructor RVA 0x004C803A; buildFieldParse RVA 0x004C7E63 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5E298 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_PLAYER_HEAL_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_PLAYER_HEAL_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char PlayerHealSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class PlayerHealSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	PlayerHealSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_healAmount;                                     // +0x7C HealAmount
	Bool m_healAsPercent;                                  // +0x80 HealAsPercent
	Real m_healRadius;                                     // +0x84 HealRadius
	KindOfMaskType m_healAffects;                          // +0x88 HealAffects; BitFlags<218>
	const FXList * m_healFX;                               // +0xA4 HealFX
	const ObjectCreationList * m_healOCL;                  // +0xA8 HealOCL
};
// SIZE 0xAC
typedef char PlayerHealSpecialPowerModuleData_size[(sizeof(PlayerHealSpecialPowerModuleData) == 0xAC) ? 1 : -1];
typedef char PlayerHealSpecialPowerModuleData_off_m_healAmount[(offsetof(PlayerHealSpecialPowerModuleData, m_healAmount) == 0x7C) ? 1 : -1];
typedef char PlayerHealSpecialPowerModuleData_off_m_healAsPercent[(offsetof(PlayerHealSpecialPowerModuleData, m_healAsPercent) == 0x80) ? 1 : -1];
typedef char PlayerHealSpecialPowerModuleData_off_m_healRadius[(offsetof(PlayerHealSpecialPowerModuleData, m_healRadius) == 0x84) ? 1 : -1];
typedef char PlayerHealSpecialPowerModuleData_off_m_healAffects[(offsetof(PlayerHealSpecialPowerModuleData, m_healAffects) == 0x88) ? 1 : -1];
typedef char PlayerHealSpecialPowerModuleData_off_m_healFX[(offsetof(PlayerHealSpecialPowerModuleData, m_healFX) == 0xA4) ? 1 : -1];
typedef char PlayerHealSpecialPowerModuleData_off_m_healOCL[(offsetof(PlayerHealSpecialPowerModuleData, m_healOCL) == 0xA8) ? 1 : -1];

#endif
