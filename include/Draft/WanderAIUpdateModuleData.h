// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BEF580).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_conditionForEntry.
// WanderAIUpdateModuleData.h: the INI data of WanderAIUpdate. SIZE 0x74.
// Target facts: ModuleFactory registers the module with operator new 0x74 and
// constructor RVA 0x0024F1CD; buildFieldParse RVA 0x0024F1F2 calls AIUpdateModuleData::buildFieldParse, then adds VA 0x00BEF580 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_conditionForEntry +0x68: INI::iniParseAnim 0x004B6481
#pragma once
#ifndef BFME2_WANDER_A_I_UPDATE_MODULE_DATA_H
#define BFME2_WANDER_A_I_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class INI;
class MultiIniFieldParse;

// Base AIUpdateModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char WanderAIUpdateModuleData_base_size[(sizeof(AIUpdateModuleData) == 0x64) ? 1 : -1];

class WanderAIUpdateModuleData : public AIUpdateModuleData
{
public:
	WanderAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Bool m_wildBeast;                                      // +0x64 WildBeast
	UnsignedByte m_unk0x65[0x3];
	Int m_conditionForEntry;                               // +0x68 ConditionForEntry (-1 = none; WanderAIUpdate::update 0x004A9424 passes it to Object::isKindOf 0x0006F039); parsed by INI::iniParseAnim 0x004B6481
	Bool m_selectable;                                     // +0x6C Selectable
	UnsignedInt m_wanderDistance;                          // +0x70 WanderDistance
};
// SIZE 0x74
typedef char WanderAIUpdateModuleData_size[(sizeof(WanderAIUpdateModuleData) == 0x74) ? 1 : -1];
typedef char WanderAIUpdateModuleData_off_m_wildBeast[(offsetof(WanderAIUpdateModuleData, m_wildBeast) == 0x64) ? 1 : -1];
typedef char WanderAIUpdateModuleData_off_m_conditionForEntry[(offsetof(WanderAIUpdateModuleData, m_conditionForEntry) == 0x68) ? 1 : -1];
typedef char WanderAIUpdateModuleData_off_m_selectable[(offsetof(WanderAIUpdateModuleData, m_selectable) == 0x6C) ? 1 : -1];
typedef char WanderAIUpdateModuleData_off_m_wanderDistance[(offsetof(WanderAIUpdateModuleData, m_wanderDistance) == 0x70) ? 1 : -1];

#endif
