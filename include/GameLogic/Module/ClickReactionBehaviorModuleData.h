// ClickReactionBehaviorModuleData.h: the INI data of ClickReactionBehavior. SIZE 0x20.
// Target facts: ModuleFactory registers the module with operator new 0x20 and
// constructor RVA 0x004595F0; buildFieldParse RVA 0x0045960C adds VA 0x00C412F8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CLICK_REACTION_BEHAVIOR_MODULE_DATA_H
#define BFME2_CLICK_REACTION_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class ClickReactionBehaviorModuleData : public ModuleData
{
public:
	ClickReactionBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Int m_clickReactionTimer;                              // +0x8 ClickReactionTimer
	Int m_reactionFrames1;                                 // +0xC ReactionFrames1
	Int m_reactionFrames2;                                 // +0x10 ReactionFrames2
	Int m_reactionFrames3;                                 // +0x14 ReactionFrames3
	Int m_reactionFrames4;                                 // +0x18 ReactionFrames4
	Int m_reactionFrames5;                                 // +0x1C ReactionFrames5
};
// SIZE 0x20
typedef char ClickReactionBehaviorModuleData_size[(sizeof(ClickReactionBehaviorModuleData) == 0x20) ? 1 : -1];
typedef char ClickReactionBehaviorModuleData_off_m_clickReactionTimer[(offsetof(ClickReactionBehaviorModuleData, m_clickReactionTimer) == 0x8) ? 1 : -1];
typedef char ClickReactionBehaviorModuleData_off_m_reactionFrames1[(offsetof(ClickReactionBehaviorModuleData, m_reactionFrames1) == 0xC) ? 1 : -1];
typedef char ClickReactionBehaviorModuleData_off_m_reactionFrames2[(offsetof(ClickReactionBehaviorModuleData, m_reactionFrames2) == 0x10) ? 1 : -1];
typedef char ClickReactionBehaviorModuleData_off_m_reactionFrames3[(offsetof(ClickReactionBehaviorModuleData, m_reactionFrames3) == 0x14) ? 1 : -1];
typedef char ClickReactionBehaviorModuleData_off_m_reactionFrames4[(offsetof(ClickReactionBehaviorModuleData, m_reactionFrames4) == 0x18) ? 1 : -1];
typedef char ClickReactionBehaviorModuleData_off_m_reactionFrames5[(offsetof(ClickReactionBehaviorModuleData, m_reactionFrames5) == 0x1C) ? 1 : -1];

#endif
