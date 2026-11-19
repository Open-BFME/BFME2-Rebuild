// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5C468).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_invisibilityNugget.
// InvisibilitySpecialPowerModuleData.h: the INI data of InvisibilitySpecialPower. SIZE 0x140.
// Target facts: ModuleFactory registers the module with operator new 0x140 and
// constructor RVA 0x004C244D; buildFieldParse RVA 0x004C2389 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5C468 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_invisibilityNugget +0x7C: Rva00438176Parse 0x00438176
#pragma once
#ifndef BFME2_INVISIBILITY_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_INVISIBILITY_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char InvisibilitySpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class InvisibilitySpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	InvisibilitySpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_invisibilityNugget[0xB8];               // +0x7C InvisibilityNugget; unresolved: Rva00438176Parse
	Real m_broadcastRadius;                                // +0x134 BroadcastRadius
	ObjectFilter m_objectFilter;                           // +0x138 ObjectFilter
	UnsignedInt m_duration;                                // +0x13C Duration; duration, frames
};
// SIZE 0x140
typedef char InvisibilitySpecialPowerModuleData_size[(sizeof(InvisibilitySpecialPowerModuleData) == 0x140) ? 1 : -1];
typedef char InvisibilitySpecialPowerModuleData_off_m_invisibilityNugget[(offsetof(InvisibilitySpecialPowerModuleData, m_invisibilityNugget) == 0x7C) ? 1 : -1];
typedef char InvisibilitySpecialPowerModuleData_off_m_broadcastRadius[(offsetof(InvisibilitySpecialPowerModuleData, m_broadcastRadius) == 0x134) ? 1 : -1];
typedef char InvisibilitySpecialPowerModuleData_off_m_objectFilter[(offsetof(InvisibilitySpecialPowerModuleData, m_objectFilter) == 0x138) ? 1 : -1];
typedef char InvisibilitySpecialPowerModuleData_off_m_duration[(offsetof(InvisibilitySpecialPowerModuleData, m_duration) == 0x13C) ? 1 : -1];

#endif
