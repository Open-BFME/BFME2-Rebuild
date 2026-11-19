// TunnelTracker.h: one cave network's shared contents (CaveSystem owns one per
// cave index). SIZE 0x28.
//
// Target facts: the constructor 0x004F55A0 installs Snapshot's table
// 0x00C63234 at +0x00 and a one-slot interface at +0x04 (its own pure table
// 0x00C1C780, then 0x00C63230), builds three STLport lists at +0x08, +0x10 and
// +0x14 and zeroes +0x0C and +0x18..+0x24. The +0x04 slot 0x004F5391 takes a
// bool and increments (true) or decrements (false) the count at +0x0C.
// 0x004F56FC (a tunnel was created) increments +0x1C and pushes the object's ID
// onto the +0x08 list; 0x004F571B is its counterpart. CaveContain's
// onBuildComplete, tryToSetCaveIndex and onDie call them.
// addToContainList 0x004F56E5 pushes the Object* onto +0x10 and increments
// +0x18; getContainMax 0x004F5385 returns TheGlobalData's MaxTunnelCapacity;
// healObjects 0x004F558C runs iterateContained with the static healObject.
// GetSnapshotName 0x004F5617, xfer 0x004F579C and loadPostProcess 0x004F585C are
// the Snapshot overrides.
// Donor facts: Zero Hour's TunnelTracker names onTunnelCreated/onTunnelDestroyed, m_tunnelIDs, m_containList,
// m_xferContainList, m_containListSize, m_tunnelCount, m_curNemesisID and
// m_nemesisTimestamp; the BFME 2 offsets follow the constructor, not the donor.
// PROVISIONAL: TunnelCountInterface / adjustTunnelCount are descriptive names
// for the +0x04 interface and its one slot; nothing in the map or WorldBuilder
// names them.
#pragma once
#ifndef BFME2_TUNNELTRACKER_H
#define BFME2_TUNNELTRACKER_H

#include "GameLogic/ContainmentListView.h"
#include <stddef.h>
#include <list>
#include "Common/GameType.h"
#include "Common/Snapshot.h"

class Object;
class ObjectFilter;
class Xfer;

typedef void (*ContainIterateFunc)(Object *obj, void *userData);

// PROVISIONAL name (see above).
class TunnelCountInterface
{
public:
	virtual void adjustTunnelCount(Bool add) = 0;						// 0
};

class TunnelTracker : public Snapshot, public TunnelCountInterface	// +0x04
{
public:
	TunnelTracker();													// 0x004F55A0

	virtual void adjustTunnelCount(Bool add);							// 0x004F5391

	ContainmentListView getContainedItemsList();					// 0x00466398 (map: ?rva00466398@Rva00466398@@QAE?AVRva0036AE51ListView@@XZ): a view of m_containList
	Bool isInContainer(Object *obj);									// 0x004F550A (the map spells the argument as an ObjectID; retail passes the object pointer)
	void removeFromContain(Object *obj, Bool exposeStealthUnits);	// 0x004F54BF (same note)
	Int getContainMax() const;											// 0x004F5385
	// With no filter, count all passengers using the cached list size.
	Int getContainCount(const ObjectFilter *filter) const;
	Bool isValidContainerFor(Object *obj, Bool checkCapacity);			// 0x004F5473
	void iterateContained(ContainIterateFunc func, void *userData, Bool reverse);	// 0x004F553F
	void healObjects(Real frames);										// 0x004F558C
	void addToContainList(Object *obj);									// 0x004F56E5
	Object *getCurNemesis();											// 0x004F5684
	void onTunnelCreated(const Object *newTunnel);						// 0x004F56FC (donor name; map placeholder)
	Bool onTunnelDestroyed(const Object *deadTunnel);					// 0x004F571B (donor name; map placeholder)
	void updateNemesis(const Object *target);							// 0x004F5935

	static void healObject(Object *obj, void *frames);					// 0x004F53C3

protected:
	virtual ~TunnelTracker();											// 0x004F561D
	virtual void loadPostProcess();										// 0x004F585C
	virtual const char *GetSnapshotName() const;						// 0x004F5617
	virtual void xfer(Xfer *xfer);										// 0x004F579C

	friend class CaveSystem;	// CaveSystem::reset 0x0042767F deletes its trackers

public:
	std::list<ObjectID> m_tunnelIDs;									// 0x08
	Int m_unk0x0C;														// 0x0C, adjusted through the +0x04 interface
	std::list<Object *> m_containList;									// 0x10
	std::list<ObjectID> m_xferContainList;								// 0x14
	Int m_containListSize;												// 0x18
	UnsignedInt m_tunnelCount;													// 0x1C, incremented by onTunnelCreated; xfer 0x004F579C moves it through the unsigned slot
	ObjectID m_unk0x20;														// 0x20 the nemesis ID (xfer 0x004F579C moves it through XferObjectID)
	UnsignedInt m_unk0x24;														// 0x24 the nemesis timestamp (xfer: unsigned slot)
};
// SIZE 0x28

typedef char TunnelTracker_off_m_tunnelCount[(offsetof(TunnelTracker, m_tunnelCount) == 0x1C) ? 1 : -1];
typedef char TunnelTracker_off_m_containList[(offsetof(TunnelTracker, m_containList) == 0x10) ? 1 : -1];
typedef char TunnelTracker_off_m_containListSize[(offsetof(TunnelTracker, m_containListSize) == 0x18) ? 1 : -1];
typedef char TunnelTracker_size[(sizeof(TunnelTracker) == 0x28) ? 1 : -1];

#endif
