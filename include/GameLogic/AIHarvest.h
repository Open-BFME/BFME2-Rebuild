// AIHarvest.h: the harvesting states (AIHarvest.cpp).
// AIHarvestPrepareSiteState and AIHarvestWorkSiteState (SIZE 0x24 each): ctors
// 0x00544C03 / 0x00544C2A call State(machine, 0xA2C0BF2B / 0x466E7761), zero the
// frame at +0x20 and store tables 0x00C69C98 / 0x00C69CF8; both xfer through the
// shared body 0x00544AB3 (Version1, UnsignedInt +0x20). onEnter 0x00544E3B /
// 0x00544E76 set +0x20 to the current frame plus the supply interface's
// preparation / action time, failing when the owner's AI has no supply interface.
#pragma once
#ifndef BFME2_AIHARVEST_H
#define BFME2_AIHARVEST_H

#include "Common/StateMachine.h"
#include "GameLogic/AIStateMachine.h"

class AIHarvestPrepareSiteState : public State
{
public:
	AIHarvestPrepareSiteState(StateMachine *machine);	// 0x00544C03
	virtual StateReturnType onEnter();					// 0x00544E3B
	virtual void onExit(StateExitType status);			// 0x005450B5
	virtual StateReturnType update();					// 0x00544FC6
	virtual void xfer(Xfer *xfer);						// 0x00544AB3 (shared; the map spells it public, UAE)

protected:
	virtual const char *GetSnapshotName() const;		// 0x00544C24

	UnsignedInt m_actionFrame;							// +0x20
};
// SIZE 0x24

class AIHarvestWorkSiteState : public State
{
public:
	AIHarvestWorkSiteState(StateMachine *machine);		// 0x00544C2A
	virtual StateReturnType onEnter();					// 0x00544E76
	virtual void onExit(StateExitType status);			// 0x005451F3
	virtual StateReturnType update();					// 0x005450D3

protected:
	virtual const char *GetSnapshotName() const;		// 0x00544C4B
	virtual void xfer(Xfer *xfer);						// 0x00544AB3 (shared)

	UnsignedInt m_actionFrame;							// +0x20
};
// SIZE 0x24

// AIHarvestMachine: the harvesting state machine. Target facts: table 0x00C69D58 (16 slots) is StateMachine's with
// slot 0 (deleting destructor 0x00544FAA), slot 2 0x00544C5C (returns "AIHarvestMachine": GetSnapshotName) and slot 3
// the xfer fold 0x0033FB0B replaced; the destructor 0x00544C51 stores the table and tail-jumps to ~StateMachine
// 0x004D759C. Constructor 0x00544ED4. Allocation size not recovered.
class AIHarvestMachine : public StateMachine
{
public:
	virtual ~AIHarvestMachine() {}						// 0x00544C51 (COMDAT)

protected:
	virtual const char *GetSnapshotName() const;		// slot 2, 0x00544C5C
	virtual void xfer(Xfer *xfer);						// slot 3, the fold 0x0033FB0B
};

// AIHarvestApproachSiteState: the move to the harvest site. Target facts: constructor 0x00544EB1 calls
// AIInternalMoveToState(machine, 0x29E75AEF) and stores table 0x00C69E08 (18 slots): slot 2 0x00544ECE (returns
// "AIHarvestApproachSiteState"), slot 3 the shared empty xfer 0x0047A69C, slot 4 onEnter 0x00544C62, slot 5 the folded
// onExit forwarder 0x0034BA5F, slot 6 update 0x00349D8D; the rest are AIInternalMoveToState's.
class AIHarvestApproachSiteState : public AIInternalMoveToState
{
public:
	AIHarvestApproachSiteState(StateMachine *machine) : AIInternalMoveToState(machine, 0x29E75AEF) {}	// 0x00544EB1
	virtual StateReturnType onEnter();					// slot 4, 0x00544C62
	virtual void onExit(StateExitType status);			// slot 5, 0x0034BA5F
	virtual StateReturnType update();					// slot 6, 0x00349D8D

protected:
	virtual const char *GetSnapshotName() const;		// slot 2, 0x00544ECE
	virtual void xfer(Xfer *xfer);						// slot 3, 0x0047A69C
};

#endif
