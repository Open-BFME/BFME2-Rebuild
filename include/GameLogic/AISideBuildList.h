// AISideBuildList.h: one side's build list (the AI data's SideInfo chain).
// Target facts: AISideBuildList::addInfo 0x002FD766 (43 bytes) takes the list
// head at +0x08. An empty list takes the new entry as its head; otherwise the
// entry is appended after the last one by following BuildListInfo's link at
// +0x2C, and the new entry's link is cleared.
#pragma once
#ifndef BFME2_AISIDEBUILDLIST_H
#define BFME2_AISIDEBUILDLIST_H

#include "Common/GameType.h"

class BuildListInfo;

class AISideBuildList
{
public:
	void addInfo(BuildListInfo *info);				// 0x002FD766

protected:
	UnsignedByte m_unk0x00[0x8];					// +0x00, not touched by addInfo
	BuildListInfo *m_head;							// +0x08, first entry of the chain
};

#endif
