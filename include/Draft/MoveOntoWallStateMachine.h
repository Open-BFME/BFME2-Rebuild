// MoveOntoWallStateMachine: one of the sub-machines the AI states build (Draft: allocation size and constructor signature not recovered).
// Target facts: table 0x00811D50 (16 slots) is StateMachine's with slots 0 (the deleting destructor), 2 and 3 replaced:
// slot 2 0x0033FC15 returns "MoveOntoWallStateMachine" (GetSnapshotName; the string is retail's spelling), slot 3 is the xfer fold 0x0033FB0B
// several machines share (StateMachine::xfer, then version 1). The destructor 0x0033FC0A (a COMDAT of AIStates.cpp)
// stores the table and tail-jumps to ~StateMachine 0x004D759C, so the class adds no members that need destruction.
// Its constructor is 0x00343BC9.
#pragma once
#include "Common/StateMachine.h"

class MoveOntoWallStateMachine : public StateMachine
{
public:
	virtual ~MoveOntoWallStateMachine() {}		// 0x0033FC0A (COMDAT)

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x0033FC15
	virtual void xfer(Xfer *xfer);					// slot 3, the fold 0x0033FB0B
};
