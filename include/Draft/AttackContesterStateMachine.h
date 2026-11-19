// AttackContesterStateMachine: one of the attack/cower sub-machines the AI states build.
// Target facts: table 0x00811BD0 (16 slots) is StateMachine's 0x00860740 with three
// slots replaced: 0 the deleting dtor 0x0034381C, 2 0x0033FB8F (returns "AttackContesterStateMachine",
// GetSnapshotName) and 3 0x0033FB0B : the xfer body several machines share.
// The dtor 0x0033FB84 stores the table and tail-jumps to ~StateMachine 0x004D759C, so
// the class adds no members that need destruction. Constructor 0x00343756.
// The map files slot 2's body as a non-virtual name() (QBE); both are declared.
// DRAFT: allocation size not recovered.
#pragma once
#include "Common/StateMachine.h"

class AttackContesterStateMachine : public StateMachine
{
public:
	AttackContesterStateMachine(Object *owner, Int, UnsignedInt);	// 0x00343756
	virtual ~AttackContesterStateMachine();	// 0x0033FB84
	const char *name() const;	// 0x0033FB8F

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x0033FB8F
	virtual void xfer(Xfer *xfer);	// slot 3, 0x0033FB0B
};
