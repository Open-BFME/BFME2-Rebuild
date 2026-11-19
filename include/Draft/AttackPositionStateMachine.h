// AttackPositionStateMachine: one of the attack/cower sub-machines the AI states build.
// Target facts: table 0x00811C90 (16 slots) is StateMachine's 0x00860740 with three
// slots replaced: 0 the deleting dtor 0x00343A6A, 2 0x0033FBB1 (returns "AttackPositionStateMachine",
// GetSnapshotName) and 3 0x0033FB0B : the xfer body several machines share.
// The dtor 0x0033FBA6 stores the table and tail-jumps to ~StateMachine 0x004D759C, so
// the class adds no members that need destruction.
// The map files slot 2's body as a non-virtual name() (QBE); both are declared.
// DRAFT: allocation size not recovered.
#pragma once
#include "Common/StateMachine.h"

class AttackPositionStateMachine : public StateMachine
{
public:
	virtual ~AttackPositionStateMachine();	// 0x0033FBA6
	const char *name() const;	// 0x0033FBB1

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x0033FBB1
	virtual void xfer(Xfer *xfer);	// slot 3, 0x0033FB0B
};
