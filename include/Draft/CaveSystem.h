// CaveSystem.h: the cave subsystem.
//
// Target facts: the constructor 0x004275D4 runs SubsystemInterface's
// constructor (0x001B4E63), installs the two tables (0x00C3C644 primary,
// 0x00C3C634 the Snapshot base at +0x0C) and constructs the tracker vector at
// +0x10. getTunnelTrackerForCaveIndex 0x004275B6 reads the vector's start at
// +0x10 and finish at +0x14 (element size four, count = (finish-start)/4);
// reset 0x0042767F deletes each tracker and clears the vector.
#pragma once
#ifndef BFME2_CAVESYSTEM_H
#define BFME2_CAVESYSTEM_H

#include <vector>
#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"
#include "Common/TunnelTracker.h"

typedef int Int;
typedef unsigned int UnsignedInt;

class Xfer;

class CaveSystem : public SubsystemInterface, public Snapshot
{
public:
	CaveSystem(void);
	virtual ~CaveSystem(void);

	virtual void init(void) {}		// SubsystemInterface slot 1: retail's table holds the folded empty body 0x000B3FD0
	virtual void update(void) {}	// slot 10: the same folded empty body
	void reset(void);
	TunnelTracker *getTunnelTrackerForCaveIndex(UnsignedInt index);
	Bool canSwitchIndexToIndex(UnsignedInt oldIndex, UnsignedInt newIndex);	// 0x0042755D (donor name; map placeholder)
	void registerNewCave(Int theIndex);
	void unregisterCave(Int theIndex);

protected:
	virtual void xfer(Xfer *xfer);	// Snapshot slot 3, 0x00427738 (the ledger spells it DoXfer, a new virtual, which cannot sit in the Snapshot table)
	virtual void loadPostProcess(void) {}	// Snapshot slot 1: retail's table holds the folded empty body 0x000B3FD0 here
	virtual const char *GetSnapshotName(void) const;

private:
	std::vector<TunnelTracker *> m_tunnelTrackerVector;	// +0x10
};

extern CaveSystem *TheCaveSystem;		// ?TheCaveSystem@@3PAVCaveSystem@@A

#endif
