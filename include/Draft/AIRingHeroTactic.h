// AIRingHeroTactic.h: the ring-hero tactic. Only the size is shown so far: the create row (0x005AC8F2) allocates 0x68
// bytes, so the 0x10 bytes after AITactic's 0x58 are not typed yet. Its constructor (0x005AC7EC) builds a name string and
// calls a placeholder class's constructor, so the real base is not named yet; this draft derives AITactic for now.
#pragma once
#ifndef BFME2_AIRINGHEROTACTIC_H
#define BFME2_AIRINGHEROTACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"

class AIRingHeroTactic : public AITactic
{
public:
	AIRingHeroTactic();									// 0x005AC7EC
	virtual Bool canRun(void *request);					// the base pure slots the class overrides (rows not written)
	virtual void cleanUp();
	virtual void run();
	virtual void update();
	virtual AITactic *create();							// 0x005AC8F2, non-base slot 0x24

	unsigned char m_unk0x58[0x10];						// +0x58..+0x68: not shown
};
// SIZE 0x68
typedef char AIRingHeroTactic_size[(sizeof(AIRingHeroTactic) == 0x68) ? 1 : -1];

#endif
