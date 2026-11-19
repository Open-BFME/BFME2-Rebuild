// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C561D0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_respawnRules.
// RespawnUpdateModuleData.h: the INI data of RespawnUpdate. SIZE 0x120.
// Target facts: ModuleFactory registers the module with operator new 0x120 and
// constructor RVA 0x004AFB4E; buildFieldParse RVA 0x004B0153 adds VA 0x00C561D0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_respawnRules +0x10C: RespawnUpdate::iniParseDefaultRule 0x004AFC5F
#pragma once
#ifndef BFME2_RESPAWN_UPDATE_MODULE_DATA_H
#define BFME2_RESPAWN_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/BitFlags.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;

#if defined(_STLPORT_VERSION)
#include <set>
#endif

// One respawn rule, the 20-byte record of the rule tree (the ledger's BfmePod20): the rules are ordered by
// level (the tree's less<> compares the first dword) and the tree is looked up with a key whose other
// fields are 0, 0, 1.0f and false (calcCostToBuild 0x004AF531, calcTimeToBuild 0x004AF5AC).
struct RespawnRule
{
	Int m_level;				// +0x00 the rule's level (the ordering key)
	Int m_cost;					// +0x04 read by calcCostToBuild
	Int m_time;					// +0x08 read by calcTimeToBuild (milliseconds, divided by 1000)
	Real m_unk0x0C;				// +0x0C 1.0f in lookup keys
	Bool m_unk0x10;				// +0x10 false in lookup keys
	bool operator<(const RespawnRule &other) const { return m_level < other.m_level; }
};

class RespawnUpdateModuleData : public ModuleData
{
public:
	RespawnUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	Int calcCostToBuild(Int level);							// 0x004AF531
	Int calcTimeToBuild(UnsignedInt level);					// 0x004AF5AC

	ObjectFilter m_autoRespawnAtObjectFilter;              // +0x8 AutoRespawnAtObjectFilter
	BitFlags<591> m_deathAnim;                             // +0xC DeathAnim; BitFlags<591>
	BitFlags<591> m_respawnAnim;                           // +0x58 RespawnAnim; BitFlags<591>
	BitFlags<591> m_initialSpawnAnim;                      // +0xA4 InitialSpawnAnim; BitFlags<591>
	const FXList * m_deathFX;                              // +0xF0 DeathFX
	const FXList * m_respawnFX;                            // +0xF4 RespawnFX
	const FXList * m_initialSpawnFX;                       // +0xF8 InitialSpawnFX
	UnsignedInt m_deathAnimationTime;                      // +0xFC DeathAnimationTime; duration, frames
	UnsignedInt m_respawnAnimationTime;                    // +0x100 RespawnAnimationTime; duration, frames
	UnsignedInt m_initialSpawnAnimationTime;               // +0x104 InitialSpawnAnimationTime; duration, frames
	UnsignedByte m_unk0x108[0x4];
#if defined(_STLPORT_VERSION)
	_STL::set<RespawnRule> m_respawnRules;                 // +0x10C RespawnRules (the rule tree: calcCostToBuild and calcTimeToBuild find in it)
#else
	UnsignedByte m_respawnRules[0xC];                      // +0x10C RespawnRules; _STL::set<RespawnRule> under stlport46
#endif
	AsciiString m_buttonImage;                             // +0x118 ButtonImage
	AsciiString m_respawnAsTemplate;                       // +0x11C RespawnAsTemplate
};
// SIZE 0x120
typedef char RespawnUpdateModuleData_size[(sizeof(RespawnUpdateModuleData) == 0x120) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_autoRespawnAtObjectFilter[(offsetof(RespawnUpdateModuleData, m_autoRespawnAtObjectFilter) == 0x8) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_deathAnim[(offsetof(RespawnUpdateModuleData, m_deathAnim) == 0xC) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_respawnAnim[(offsetof(RespawnUpdateModuleData, m_respawnAnim) == 0x58) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_initialSpawnAnim[(offsetof(RespawnUpdateModuleData, m_initialSpawnAnim) == 0xA4) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_deathFX[(offsetof(RespawnUpdateModuleData, m_deathFX) == 0xF0) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_respawnFX[(offsetof(RespawnUpdateModuleData, m_respawnFX) == 0xF4) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_initialSpawnFX[(offsetof(RespawnUpdateModuleData, m_initialSpawnFX) == 0xF8) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_deathAnimationTime[(offsetof(RespawnUpdateModuleData, m_deathAnimationTime) == 0xFC) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_respawnAnimationTime[(offsetof(RespawnUpdateModuleData, m_respawnAnimationTime) == 0x100) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_initialSpawnAnimationTime[(offsetof(RespawnUpdateModuleData, m_initialSpawnAnimationTime) == 0x104) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_respawnRules[(offsetof(RespawnUpdateModuleData, m_respawnRules) == 0x10C) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_buttonImage[(offsetof(RespawnUpdateModuleData, m_buttonImage) == 0x118) ? 1 : -1];
typedef char RespawnUpdateModuleData_off_m_respawnAsTemplate[(offsetof(RespawnUpdateModuleData, m_respawnAsTemplate) == 0x11C) ? 1 : -1];

#endif
