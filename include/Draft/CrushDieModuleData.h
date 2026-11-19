// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF0868).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. The three sound handles and percents are typed (CrushDie::onDie 0x00486327).
// CrushDieModuleData.h: the INI data of CrushDie. SIZE 0x58.
// Target facts: ModuleFactory registers the module with operator new 0x58 and
// constructor RVA 0x002539B3; buildFieldParse RVA 0x00253A05 adds VA 0x00BF0868 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_totalCrushSound +0x38: INI::parseAudioEventRTS 0x00339900
//   m_backEndCrushSound +0x3C: INI::parseAudioEventRTS 0x00339900
//   m_frontEndCrushSound +0x40: INI::parseAudioEventRTS 0x00339900
#pragma once
#ifndef BFME2_CRUSH_DIE_MODULE_DATA_H
#define BFME2_CRUSH_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"
#include "Common/OpaqueRefElement4.h"
#include "Common/OpaqueRefElement4.h"

class INI;
class MultiIniFieldParse;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char CrushDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class CrushDieModuleData : public DestroyDieModuleData
{
public:
	CrushDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	// CrushDie::onDie 0x00486327 indexes both arrays by CrushEnum (total, back end, front end) and reads the
	// handle's pointer (`cmp [data+0x38+4*type],0`) before building the sound; the table's tokens are
	// TotalCrushSound +0x38, BackEndCrushSound +0x3C, FrontEndCrushSound +0x40 (INI::parseAudioEventRTS 0x00339900).
	OpaqueRefElement4 m_crushSounds[3];                    // +0x38
	UnsignedByte m_unk0x44[0x4];
	// TotalCrushSoundPercent +0x48, BackEndCrushSoundPercent +0x4C, FrontEndCrushSoundPercent +0x50.
	Int m_crushSoundPercent[3];                            // +0x48
	UnsignedByte m_unk0x54[0x4];
};
// SIZE 0x58
typedef char CrushDieModuleData_size[(sizeof(CrushDieModuleData) == 0x58) ? 1 : -1];
typedef char CrushDieModuleData_off_m_crushSounds[(offsetof(CrushDieModuleData, m_crushSounds) == 0x38) ? 1 : -1];
typedef char CrushDieModuleData_off_m_crushSoundPercent[(offsetof(CrushDieModuleData, m_crushSoundPercent) == 0x48) ? 1 : -1];

#endif
