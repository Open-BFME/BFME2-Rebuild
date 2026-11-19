// FPModeScope.h: a scope that holds the logic's floating point mode.
// Target facts (retail game.dat): the constructor 0x0004224C (map: Rva0004224C::rva0004224C)
// returns `this`; when TheGameLogic exists it calls GameLogic's 0x000421FD, which calls
// setFPMode() (0x00040EA9: the x87 control word) when the counter at GameLogic +0x1B4 is 0 and
// then increments the counter. The destructor is inline in its users (gameStatePostProcessLoad
// 0x002DC52B ends with `dec [TheGameLogic+0x1B4]` behind a null test), so it only drops the count.
#pragma once
#ifndef BFME2_FPMODESCOPE_H
#define BFME2_FPMODESCOPE_H

#include "GameLogic/GameLogic.h"

class FPModeScope
{
public:
	FPModeScope();										// 0x0004224C
	~FPModeScope()
	{
		if (TheGameLogic)
			--TheGameLogic->m_unk0x1B4;
	}
};

#endif
