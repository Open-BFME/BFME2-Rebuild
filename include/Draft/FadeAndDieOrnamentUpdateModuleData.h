// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF2358).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_envelope, m_followTarget.
// FadeAndDieOrnamentUpdateModuleData.h: the INI data of FadeAndDieOrnamentUpdate. SIZE 0x38.
// Target facts: ModuleFactory registers the module with operator new 0x38 and
// constructor RVA 0x002555AC; buildFieldParse RVA 0x0025438C adds VA 0x00BF2358 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_envelope +0xC: INI::Rva00338E52_ParseEnvelope 0x00338E52
//   m_followTarget +0x34: INI::Rva00338E52_ParseEnvelope 0x00338E52
#pragma once
#ifndef BFME2_FADE_AND_DIE_ORNAMENT_UPDATE_MODULE_DATA_H
#define BFME2_FADE_AND_DIE_ORNAMENT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class FadeAndDieOrnamentUpdateModuleData : public ModuleData
{
public:
	FadeAndDieOrnamentUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_attachToTargetBone;                      // +0x8 AttachToTargetBone
	UnsignedByte m_envelope[0x28];                         // +0xC Envelope; unresolved: INI::Rva00338E52_ParseEnvelope
	UnsignedByte m_followTarget[0x4];                      // +0x34 FollowTarget; unresolved: INI::Rva00338E52_ParseEnvelope
};
// SIZE 0x38
typedef char FadeAndDieOrnamentUpdateModuleData_size[(sizeof(FadeAndDieOrnamentUpdateModuleData) == 0x38) ? 1 : -1];
typedef char FadeAndDieOrnamentUpdateModuleData_off_m_attachToTargetBone[(offsetof(FadeAndDieOrnamentUpdateModuleData, m_attachToTargetBone) == 0x8) ? 1 : -1];
typedef char FadeAndDieOrnamentUpdateModuleData_off_m_envelope[(offsetof(FadeAndDieOrnamentUpdateModuleData, m_envelope) == 0xC) ? 1 : -1];
typedef char FadeAndDieOrnamentUpdateModuleData_off_m_followTarget[(offsetof(FadeAndDieOrnamentUpdateModuleData, m_followTarget) == 0x34) ? 1 : -1];

#endif
