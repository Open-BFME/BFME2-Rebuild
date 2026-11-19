// SkirmishGameInfo.h: a GameInfo that owns its eight GameSlots (the skirmish and replay set-up record).
// Evidence (binary): constructor 0x0022C45B runs GameInfo's constructor 0x00400A07, stores the table
// VA 0x00BE7480, builds eight GameSlots at +0xDC (stride 0x1AC) and hands each to
// GameInfo::setSlotPointer (0x003FF332); the destructor 0x0022C516 destroys the eight slots
// and then runs ~GameInfo (0x00400A7F); the deleting body is 0x0022C4FA. SIZE 0xDC + 8 * 0x1AC = 0xE3C,
// which is the extent of RecorderClass's embedded record (RecorderClass +0x24, +0xE60 next member).
// Slot 2 of its table (0x0022C4D9) returns "SkirmishGameInfo": the map spells that body as a
// non-virtual `name()`, so the override is declared that way here and the table stays GameInfo's
// until the map renames the slot. Every other slot is GameInfo's.
#pragma once
#ifndef BFME2_SKIRMISHGAMEINFO_H
#define BFME2_SKIRMISHGAMEINFO_H

#include <stddef.h>
#include "GameNetwork/GameInfo.h"
#include "GameNetwork/GameSlot.h"

class SkirmishGameInfo : public GameInfo
{
public:
	SkirmishGameInfo();							// 0x0022C45B
	// the destructor 0x0022C516 (deleting 0x0022C4FA) is implicit: retail emits no vtable store in it

	const char *name() const;					// 0x0022C4D9

	GameSlot m_skirmishSlots[MAX_SLOTS];		// +0xDC
};
typedef char SkirmishGameInfoSize[(sizeof(SkirmishGameInfo) == 0xE3C) ? 1 : -1];
extern SkirmishGameInfo *TheSkirmishGameInfo;
typedef char SkirmishGameInfoSlotsOffset[(offsetof(SkirmishGameInfo, m_skirmishSlots) == 0xDC) ? 1 : -1];

#endif
