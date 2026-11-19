// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C511A0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// GiveUpgradeUpdateModuleData.h: the INI data of GiveUpgradeUpdate. SIZE 0xE4.
// Target facts: ModuleFactory registers the module with operator new 0xE4 and
// constructor RVA 0x0049C4D2; buildFieldParse RVA 0x0049C391 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C511A0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_GIVE_UPGRADE_UPDATE_MODULE_DATA_H
#define BFME2_GIVE_UPGRADE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char GiveUpgradeUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class GiveUpgradeUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	GiveUpgradeUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_unk0xC8[0xC];
	const FXList * m_giveUpgradeEffect;                    // +0xD4 GiveUpgradeEffect
	const FXList * m_spawnOutFX;                           // +0xD8 SpawnOutFX
	Real m_fadeOutSpeed;                                   // +0xDC FadeOutSpeed
	Bool m_deliverUpgrade;                                 // +0xE0 DeliverUpgrade
};
// SIZE 0xE4
typedef char GiveUpgradeUpdateModuleData_size[(sizeof(GiveUpgradeUpdateModuleData) == 0xE4) ? 1 : -1];
typedef char GiveUpgradeUpdateModuleData_off_m_giveUpgradeEffect[(offsetof(GiveUpgradeUpdateModuleData, m_giveUpgradeEffect) == 0xD4) ? 1 : -1];
typedef char GiveUpgradeUpdateModuleData_off_m_spawnOutFX[(offsetof(GiveUpgradeUpdateModuleData, m_spawnOutFX) == 0xD8) ? 1 : -1];
typedef char GiveUpgradeUpdateModuleData_off_m_fadeOutSpeed[(offsetof(GiveUpgradeUpdateModuleData, m_fadeOutSpeed) == 0xDC) ? 1 : -1];
typedef char GiveUpgradeUpdateModuleData_off_m_deliverUpgrade[(offsetof(GiveUpgradeUpdateModuleData, m_deliverUpgrade) == 0xE0) ? 1 : -1];

#endif
