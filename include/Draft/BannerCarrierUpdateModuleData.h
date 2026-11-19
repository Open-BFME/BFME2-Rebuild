// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C4F9E8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_morphCondition, m_expLevelDraw.
// BannerCarrierUpdateModuleData.h: the INI data of BannerCarrierUpdate. SIZE 0x44.
// Target facts: ModuleFactory registers the module with operator new 0x44 and
// constructor RVA 0x00496FA5; buildFieldParse RVA 0x004976CA adds VA 0x00C4F9E8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_morphCondition +0x18: parseBannerCarrierMorphCondition 0x0049714F
//   m_expLevelDraw +0x24: parseBannerCarrierExpLevelDraw 0x004972A9
#pragma once
#ifndef BFME2_BANNER_CARRIER_UPDATE_MODULE_DATA_H
#define BFME2_BANNER_CARRIER_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;

class BannerCarrierUpdateModuleData : public ModuleData
{
public:
	BannerCarrierUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_idleSpawnRate;                           // +0x8 IdleSpawnRate; duration, frames
	UnsignedInt m_meleeFreeUnitSpawnTime;                  // +0xC MeleeFreeUnitSpawnTime; duration, frames
	UnsignedInt m_diedRespawnTime;                         // +0x10 DiedRespawnTime; duration, frames
	UnsignedInt m_meleeFreeBannerReSpawnTime;              // +0x14 MeleeFreeBannerReSpawnTime; duration, frames
	UnsignedByte m_morphCondition[0xC];                    // +0x18 MorphCondition; unresolved: parseBannerCarrierMorphCondition
	UnsignedByte m_expLevelDraw[0xC];                      // +0x24 ExpLevelDraw; unresolved: parseBannerCarrierExpLevelDraw
	const FXList * m_bannerMorphFX;                        // +0x30 BannerMorphFX
	const FXList * m_unitSpawnFX;                          // +0x34 UnitSpawnFX
	Bool m_replenishNearbyHorde;                           // +0x38 ReplenishNearbyHorde
	Bool m_replenishAllNearbyHordes;                       // +0x39 ReplenishAllNearbyHordes
	Real m_scanHordeDistance;                              // +0x3C ScanHordeDistance
	AsciiString m_upgradeRequired;                         // +0x40 UpgradeRequired
};
// SIZE 0x44
typedef char BannerCarrierUpdateModuleData_size[(sizeof(BannerCarrierUpdateModuleData) == 0x44) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_idleSpawnRate[(offsetof(BannerCarrierUpdateModuleData, m_idleSpawnRate) == 0x8) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_meleeFreeUnitSpawnTime[(offsetof(BannerCarrierUpdateModuleData, m_meleeFreeUnitSpawnTime) == 0xC) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_diedRespawnTime[(offsetof(BannerCarrierUpdateModuleData, m_diedRespawnTime) == 0x10) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_meleeFreeBannerReSpawnTime[(offsetof(BannerCarrierUpdateModuleData, m_meleeFreeBannerReSpawnTime) == 0x14) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_morphCondition[(offsetof(BannerCarrierUpdateModuleData, m_morphCondition) == 0x18) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_expLevelDraw[(offsetof(BannerCarrierUpdateModuleData, m_expLevelDraw) == 0x24) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_bannerMorphFX[(offsetof(BannerCarrierUpdateModuleData, m_bannerMorphFX) == 0x30) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_unitSpawnFX[(offsetof(BannerCarrierUpdateModuleData, m_unitSpawnFX) == 0x34) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_replenishNearbyHorde[(offsetof(BannerCarrierUpdateModuleData, m_replenishNearbyHorde) == 0x38) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_replenishAllNearbyHordes[(offsetof(BannerCarrierUpdateModuleData, m_replenishAllNearbyHordes) == 0x39) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_scanHordeDistance[(offsetof(BannerCarrierUpdateModuleData, m_scanHordeDistance) == 0x3C) ? 1 : -1];
typedef char BannerCarrierUpdateModuleData_off_m_upgradeRequired[(offsetof(BannerCarrierUpdateModuleData, m_upgradeRequired) == 0x40) ? 1 : -1];

#endif
