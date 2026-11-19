// AttackHordeStateMachine: one of the sub-machines the AI states build (Draft: allocation size and constructor signature not recovered).
// Target facts: table 0x00811B18 (16 slots) is StateMachine's with slots 0 (the deleting destructor), 2 and 3 replaced:
// slot 2 0x0033FB6D returns "AttackHordeStateMachine" (GetSnapshotName; the string is retail's spelling), slot 3 is the xfer fold 0x0033FB0B
// several machines share (StateMachine::xfer, then version 1). The destructor 0x0033FB20 (a COMDAT of AIStates.cpp)
// stores the table and tail-jumps to ~StateMachine 0x004D759C, so the class adds no members that need destruction.
// Its constructor is 0x00343367.
#pragma once
#include "Common/StateMachine.h"

class AttackHordeStateMachine : public StateMachine
{
public:
	virtual ~AttackHordeStateMachine() {}		// 0x0033FB20 (COMDAT)

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x0033FB6D
	virtual void xfer(Xfer *xfer);					// slot 3, the fold 0x0033FB0B
};
