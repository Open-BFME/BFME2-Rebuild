// HitReactionBehaviorModuleData.h: the INI data of HitReactionBehavior. SIZE 0x24.
// Target facts: ModuleFactory registers the module with operator new 0x24 and
// constructor RVA 0x004591EA; buildFieldParse RVA 0x00459217 adds VA 0x00C410F0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_HIT_REACTION_BEHAVIOR_MODULE_DATA_H
#define BFME2_HIT_REACTION_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class HitReactionBehaviorModuleData : public ModuleData
{
public:
	HitReactionBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_hitReactionLifeTimer1;                   // +0x8 HitReactionLifeTimer1; duration, frames
	UnsignedInt m_hitReactionLifeTimer2;                   // +0xC HitReactionLifeTimer2; duration, frames
	UnsignedInt m_hitReactionLifeTimer3;                   // +0x10 HitReactionLifeTimer3; duration, frames
	Real m_hitReactionThreshold1;                          // +0x14 HitReactionThreshold1
	Real m_hitReactionThreshold2;                          // +0x18 HitReactionThreshold2
	Real m_hitReactionThreshold3;                          // +0x1C HitReactionThreshold3
	Bool m_fastHitsResetReaction;                          // +0x20 FastHitsResetReaction
	Bool m_hitsParalyze;                                   // +0x21 HitsParalyze
};
// SIZE 0x24
typedef char HitReactionBehaviorModuleData_size[(sizeof(HitReactionBehaviorModuleData) == 0x24) ? 1 : -1];
typedef char HitReactionBehaviorModuleData_off_m_hitReactionLifeTimer1[(offsetof(HitReactionBehaviorModuleData, m_hitReactionLifeTimer1) == 0x8) ? 1 : -1];
typedef char HitReactionBehaviorModuleData_off_m_hitReactionLifeTimer2[(offsetof(HitReactionBehaviorModuleData, m_hitReactionLifeTimer2) == 0xC) ? 1 : -1];
typedef char HitReactionBehaviorModuleData_off_m_hitReactionLifeTimer3[(offsetof(HitReactionBehaviorModuleData, m_hitReactionLifeTimer3) == 0x10) ? 1 : -1];
typedef char HitReactionBehaviorModuleData_off_m_hitReactionThreshold1[(offsetof(HitReactionBehaviorModuleData, m_hitReactionThreshold1) == 0x14) ? 1 : -1];
typedef char HitReactionBehaviorModuleData_off_m_hitReactionThreshold2[(offsetof(HitReactionBehaviorModuleData, m_hitReactionThreshold2) == 0x18) ? 1 : -1];
typedef char HitReactionBehaviorModuleData_off_m_hitReactionThreshold3[(offsetof(HitReactionBehaviorModuleData, m_hitReactionThreshold3) == 0x1C) ? 1 : -1];
typedef char HitReactionBehaviorModuleData_off_m_fastHitsResetReaction[(offsetof(HitReactionBehaviorModuleData, m_fastHitsResetReaction) == 0x20) ? 1 : -1];
typedef char HitReactionBehaviorModuleData_off_m_hitsParalyze[(offsetof(HitReactionBehaviorModuleData, m_hitsParalyze) == 0x21) ? 1 : -1];

#endif
