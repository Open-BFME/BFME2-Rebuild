// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5F064).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// AnimationSoundClientBehaviorModuleData.h: the INI data of AnimationSoundClientBehavior. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x004CA70D; buildFieldParse RVA 0x004CAB2C adds VA 0x00C5F064 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ANIMATION_SOUND_CLIENT_BEHAVIOR_MODULE_DATA_H
#define BFME2_ANIMATION_SOUND_CLIENT_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class AnimationSoundClientBehaviorModuleData : public ModuleData
{
public:
	AnimationSoundClientBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_unk0x8[0xC];
	Real m_maxUpdateRangeCap;                              // +0x14 MaxUpdateRangeCap; positive, non-zero
};
// SIZE 0x18
typedef char AnimationSoundClientBehaviorModuleData_size[(sizeof(AnimationSoundClientBehaviorModuleData) == 0x18) ? 1 : -1];
typedef char AnimationSoundClientBehaviorModuleData_off_m_maxUpdateRangeCap[(offsetof(AnimationSoundClientBehaviorModuleData, m_maxUpdateRangeCap) == 0x14) ? 1 : -1];

#endif
