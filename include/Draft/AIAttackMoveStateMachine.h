// AIAttackMoveStateMachine: one of the sub-machines the AI states build (Draft: allocation size and constructor signature not recovered).
// Target facts: table 0x00811E58 (16 slots) is StateMachine's with slots 0 (the deleting destructor), 2 and 3 replaced:
// slot 2 0x00340057 returns "AIAttackMOveStateMachine" (GetSnapshotName; the string is retail's spelling), slot 3 is the xfer fold 0x0033FB0B
// several machines share (StateMachine::xfer, then version 1). The destructor 0x0034004C (a COMDAT of AIStates.cpp)
// stores the table and tail-jumps to ~StateMachine 0x004D759C, so the class adds no members that need destruction.
// Its constructor is 0x0034C6EF.
#pragma once
#include "Common/StateMachine.h"

class AIAttackMoveStateMachine : public StateMachine
{
public:
	virtual ~AIAttackMoveStateMachine() {}		// 0x0034004C (COMDAT)

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x00340057
	virtual void xfer(Xfer *xfer);					// slot 3, the fold 0x0033FB0B
};
