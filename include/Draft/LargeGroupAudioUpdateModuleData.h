// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C54868).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_key, m_unitWeight.
// LargeGroupAudioUpdateModuleData.h: the INI data of LargeGroupAudioUpdate. SIZE 0x20.
// Target facts: ModuleFactory registers the module with operator new 0x20 and
// constructor RVA 0x004ABED8; buildFieldParse RVA 0x004AB800 adds VA 0x00C54868 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_key +0x8: Rva003D4920::parseAudioKeyTokens 0x003EDBAE
//   m_unitWeight +0x1C: INI::parseUnsignedShort 0x0002EF09
#pragma once
#ifndef BFME2_LARGE_GROUP_AUDIO_UPDATE_MODULE_DATA_H
#define BFME2_LARGE_GROUP_AUDIO_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class LargeGroupAudioUpdateModuleData : public ModuleData
{
public:
	LargeGroupAudioUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_key[0xC];                               // +0x8 Key; unresolved: Rva003D4920::parseAudioKeyTokens
	UnsignedInt m_timeBetweenUpdatesMin;                   // +0x14 TimeBetweenUpdatesMin; duration, frames
	UnsignedInt m_timeBetweenUpdatesVariation;             // +0x18 TimeBetweenUpdatesVariation; duration, frames
	UnsignedByte m_unitWeight[0x4];                        // +0x1C UnitWeight; unresolved: INI::parseUnsignedShort
};
// SIZE 0x20
typedef char LargeGroupAudioUpdateModuleData_size[(sizeof(LargeGroupAudioUpdateModuleData) == 0x20) ? 1 : -1];
typedef char LargeGroupAudioUpdateModuleData_off_m_key[(offsetof(LargeGroupAudioUpdateModuleData, m_key) == 0x8) ? 1 : -1];
typedef char LargeGroupAudioUpdateModuleData_off_m_timeBetweenUpdatesMin[(offsetof(LargeGroupAudioUpdateModuleData, m_timeBetweenUpdatesMin) == 0x14) ? 1 : -1];
typedef char LargeGroupAudioUpdateModuleData_off_m_timeBetweenUpdatesVariation[(offsetof(LargeGroupAudioUpdateModuleData, m_timeBetweenUpdatesVariation) == 0x18) ? 1 : -1];
typedef char LargeGroupAudioUpdateModuleData_off_m_unitWeight[(offsetof(LargeGroupAudioUpdateModuleData, m_unitWeight) == 0x1C) ? 1 : -1];

#endif
