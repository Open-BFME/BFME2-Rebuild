// AttackFireWhileMovingMachine: one of the sub-machines the AI states build (Draft: allocation size and constructor signature not recovered).
// Target facts: table 0x00811B70 (16 slots) is StateMachine's with slots 0 (the deleting destructor), 2 and 3 replaced:
// slot 2 0x0033FB7E returns "AttackFireWhileMovingMachine" (GetSnapshotName; the string is retail's spelling), slot 3 is the xfer fold 0x0033FB0B
// several machines share (StateMachine::xfer, then version 1). The destructor 0x0033FB73 (a COMDAT of AIStates.cpp)
// stores the table and tail-jumps to ~StateMachine 0x004D759C, so the class adds no members that need destruction.
// Its constructor is 0x003435FB.
#pragma once
#include "Common/StateMachine.h"

class AttackFireWhileMovingMachine : public StateMachine
{
public:
	virtual ~AttackFireWhileMovingMachine() {}		// 0x0033FB73 (COMDAT)

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x0033FB7E
	virtual void xfer(Xfer *xfer);					// slot 3, the fold 0x0033FB0B
};
