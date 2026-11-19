// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5BECC).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DetachableRiderBodyModuleData.h: the INI data of DetachableRiderBody. SIZE 0x180.
// Target facts: ModuleFactory registers the module with operator new 0x180 and
// constructor RVA 0x004C1E8B; buildFieldParse RVA 0x004C1B80 calls ActiveBodyModuleData::buildFieldParse, then adds VA 0x00C5BECC at +0x0 and UpgradeMuxData at +0x64.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DETACHABLE_RIDER_BODY_MODULE_DATA_H
#define BFME2_DETACHABLE_RIDER_BODY_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBodyModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"

class INI;
class MultiIniFieldParse;

// Base ActiveBodyModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char DetachableRiderBodyModuleData_base_size[(sizeof(ActiveBodyModuleData) == 0x64) ? 1 : -1];

class DetachableRiderBodyModuleData : public ActiveBodyModuleData
{
public:
	DetachableRiderBodyModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x64; added by buildFieldParse at +0x64
	Real m_healthPercentageWhenRiderDies;                  // +0x174 HealthPercentageWhenRiderDies; percent
	Bool m_startsActive;                                   // +0x178 StartsActive
	Real m_riderlessDeathChance;                           // +0x17C RiderlessDeathChance; percent
};
// SIZE 0x180
typedef char DetachableRiderBodyModuleData_size[(sizeof(DetachableRiderBodyModuleData) == 0x180) ? 1 : -1];
typedef char DetachableRiderBodyModuleData_off_m_upgradeMuxData[(offsetof(DetachableRiderBodyModuleData, m_upgradeMuxData) == 0x64) ? 1 : -1];
typedef char DetachableRiderBodyModuleData_off_m_healthPercentageWhenRiderDies[(offsetof(DetachableRiderBodyModuleData, m_healthPercentageWhenRiderDies) == 0x174) ? 1 : -1];
typedef char DetachableRiderBodyModuleData_off_m_startsActive[(offsetof(DetachableRiderBodyModuleData, m_startsActive) == 0x178) ? 1 : -1];
typedef char DetachableRiderBodyModuleData_off_m_riderlessDeathChance[(offsetof(DetachableRiderBodyModuleData, m_riderlessDeathChance) == 0x17C) ? 1 : -1];

#endif
