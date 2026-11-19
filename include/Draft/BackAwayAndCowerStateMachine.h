// BackAwayAndCowerStateMachine: one of the attack/cower sub-machines the AI states build.
// Target facts: table 0x00811FE8 (16 slots) is StateMachine's 0x00860740 with three
// slots replaced: 0 the deleting dtor 0x0034457C, 2 0x003402DB (returns "BackAwayAndCowerStateMachine",
// GetSnapshotName) and 3 0x003402E1 (own xfer).
// The dtor 0x003402D0 stores the table and tail-jumps to ~StateMachine 0x004D759C, so
// the class adds no members that need destruction. Constructor 0x003444CE.
// The map files slot 2's body as a non-virtual name() (QBE); both are declared.
// DRAFT: allocation size not recovered.
#pragma once
#include "Common/StateMachine.h"

class BackAwayAndCowerStateMachine : public StateMachine
{
public:
	BackAwayAndCowerStateMachine(Object *owner, Object *, UnsignedInt);	// 0x003444CE
	virtual ~BackAwayAndCowerStateMachine();	// 0x003402D0
	const char *name() const;	// 0x003402DB

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x003402DB
	virtual void xfer(Xfer *xfer);	// slot 3, 0x003402E1
};
