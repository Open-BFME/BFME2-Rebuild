// GameResultsInterface.h: the interface of the thread that reports a finished game's results to the GameSpy servers (Zero
// Hour's GameResultsInterface by role; replaces Draft/GameResultsInterface.h).
// Target facts (code RVAs, VA 0x00C3AD08 table): the constructor 0x00419D18 runs SubsystemInterface's 0x001B4E63 and stores the
// table. The table has 22 slots: SubsystemInterface's 14 (the destructor, 0x00419CDF deleting) and eight pure virtuals at
// 0x38..0x54 whose implementations are the concrete queue's (the map names that class by its address, Rva0041A644: it has
// startThreads, endThreads, areThreadsRunning, getRequest, getResponse, addRequest, addResponse and areGameResultsBeingSent).
// The order of those eight in the table is not evidenced here, so the slots are named by their offsets.
#pragma once
#ifndef BFME2_GAMERESULTSINTERFACE_H
#define BFME2_GAMERESULTSINTERFACE_H

#include "Common/SubsystemInterface.h"

class GameResultsInterface : public SubsystemInterface
{
public:
	GameResultsInterface();										// 0x00419D18
	virtual void vslot38() = 0;									// slot 0x38
	virtual void vslot3C() = 0;									// slot 0x3C
	virtual void vslot40() = 0;									// slot 0x40
	virtual void vslot44() = 0;									// slot 0x44
	virtual void vslot48() = 0;									// slot 0x48
	virtual void vslot4C() = 0;									// slot 0x4C
	virtual void vslot50() = 0;									// slot 0x50
	virtual void vslot54() = 0;									// slot 0x54
};

#endif
