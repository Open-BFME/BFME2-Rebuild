// GODupBase.h: the snapshot/subsystem base the buff logic is built on (BuffLogic.cpp).
// Target facts (retail game.dat 1.06):
// - Constructor 0x00306782: Snapshot's table 0x007BB554 at +0, SubsystemInterface's constructor on
//   +0x4, then its own tables 0x00807D3C at +0x4 and 0x00807D2C at +0; no members of its own. The
//   destructor 0x003067D2 restores them, calls SubsystemInterface's and ends with Snapshot's table.
//   SIZE 0x10 (Snapshot's vptr and SubsystemInterface's 0xC).
// - Table 0x00807D2C (Snapshot): 0 deleting destructor 0x0030680E, 1 the shared empty body
//   0x000B3FD0, 2 GetSnapshotName 0x003067C4 (returns "BuffLogic"), 3 the version-only xfer fold
//   0x00341E2D (`if (!xfer->IsLightCRC()) xfer->Version1();`).
// - Table 0x00807D3C (SubsystemInterface, 14 slots): init (1), reset (9) and update (10) are the
//   empty body 0x000B3FD0; the rest are SubsystemInterface's own defaults.
#pragma once
#ifndef BFME2_GODUPBASE_H
#define BFME2_GODUPBASE_H

#include <stddef.h>
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"
#include "Common/Xfer.h"

class GODupBase : public Snapshot, public SubsystemInterface
{
public:
	GODupBase();												// 0x00306782
	virtual ~GODupBase();										// 0x003067D2

	virtual void init() {}										// SubsystemInterface slot 1, 0x000B3FD0
	virtual void reset() {}										// slot 9, 0x000B3FD0
	virtual void update() {}									// slot 10, 0x000B3FD0

protected:
	virtual void loadPostProcess() {}							// Snapshot slot 1, 0x000B3FD0
	virtual const char *GetSnapshotName() const;				// Snapshot slot 2, 0x003067C4
	virtual void xfer(Xfer *xfer) { if (!xfer->IsLightCRC()) xfer->Version1(); }	// Snapshot slot 3, the fold 0x00341E2D
};
// SIZE 0x10
typedef char GODupBase_size[(sizeof(GODupBase) == 0x10) ? 1 : -1];

#endif
