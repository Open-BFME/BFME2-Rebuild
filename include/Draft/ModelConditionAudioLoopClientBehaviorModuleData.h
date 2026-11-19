// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5F574).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_modelCondition.
// ModelConditionAudioLoopClientBehaviorModuleData.h: the INI data of ModelConditionAudioLoopClientBehavior. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x004CC20A; buildFieldParse RVA 0x004CC583 adds VA 0x00C5F574 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_modelCondition +0x8: Rva004CC371Parse 0x004CC371
#pragma once
#ifndef BFME2_MODEL_CONDITION_AUDIO_LOOP_CLIENT_BEHAVIOR_MODULE_DATA_H
#define BFME2_MODEL_CONDITION_AUDIO_LOOP_CLIENT_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class ModelConditionAudioLoopClientBehaviorModuleData : public ModuleData
{
public:
	ModelConditionAudioLoopClientBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_modelCondition[0xC];                    // +0x8 ModelCondition; unresolved: Rva004CC371Parse
};
// SIZE 0x14
typedef char ModelConditionAudioLoopClientBehaviorModuleData_size[(sizeof(ModelConditionAudioLoopClientBehaviorModuleData) == 0x14) ? 1 : -1];
typedef char ModelConditionAudioLoopClientBehaviorModuleData_off_m_modelCondition[(offsetof(ModelConditionAudioLoopClientBehaviorModuleData, m_modelCondition) == 0x8) ? 1 : -1];

#endif
