// Squad.h: a list of object IDs the AI keeps for a task (Zero Hour's Squad; no MemoryPoolObject base here).
// Target facts: SIZE 0x1C. Snapshot's table (deleting destructor 0x0026AF6A, empty slot 1, GetSnapshotName 0x00268CD6,
// xfer 0x004D6EC4); the ObjectID vector is at +0x04 (push_back 0x002E01C6, erase 0x00532803, assignment 0x0021C21B) and
// the cache of live Object pointers at +0x10 (clear 0x0031BD55, push_back 0x001F211B). Two out-of-line helpers refill
// the cache from the IDs: 0x004D6CAC keeps every ID, 0x004D6CF7 erases the IDs whose object is gone (Zero Hour's
// getAllObjects); getLiveObjects 0x004D6D4F takes a Bool choosing between them and drops the unselectable objects.
#pragma once
#ifndef BFME2_SQUAD_H
#define BFME2_SQUAD_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/Vector.h"

class AIGroup;
class Object;
class Team;
class Xfer;

class Squad : public Snapshot
{
public:
	// Zero Hour's names (the Draft header this replaces carried them): Player::getSquadNumberForObject 0x002AA191 and
	// removeObjectFromHotkeySquad 0x002AA1BF call isOnSquad/removeObject on each of Player's ten squads, and
	// setCurrentlySelectedAIGroup 0x002ACC53 / processSelectTeamGameMessage 0x002ACAFB call clearSquad.
	bool isOnSquad(const Object *objToFind) const;					// 0x004D6BCD
	void removeObject(Object *objectToRemove);						// 0x004D6BF0
	void clearSquad();												// 0x004D6C29
	void addObject(Object *objectToAdd);							// 0x004D6C7C
	void addObjectID(ObjectID objectID);							// 0x004D6C9C
	const _STL::vector<Object *> &getAllObjects();					// 0x004D6CF7: refills the cache, erasing the IDs of deleted objects
	const _STL::vector<Object *> &getAllObjectsKeepingIDs();		// 0x004D6CAC: refills the cache, keeping every ID
	const _STL::vector<Object *> &getLiveObjects(Bool pruneDeadIDs);	// 0x004D6D4F: the first helper when true, else the second, then drops the unselectable objects
	void squadFromAIGroup(const AIGroup *fromAIGroup, Bool clearSquadFirst);	// 0x004D6C48
	void squadFromTeam(const Team *fromTeam, Bool clearSquadFirst);	// 0x004D6D95
	void squadFromHordeRiders(Object *hordeObject, Bool clearSquadFirst);	// 0x004D6DF3 (role name): the object's horde interface lists its riders (slot 67)
	void aiGroupFromSquad(AIGroup *aiGroupToFill);					// 0x004D6E95

protected:
	virtual void loadPostProcess() {}								// slot 1: the shared `ret` 0x000B3FD0 (table 0x007F9D60)
	virtual void xfer(Xfer *xfer);									// 0x004D6EC4
	virtual const char *GetSnapshotName() const;					// 0x00268CD6

	_STL::vector<ObjectID> m_objectIDs;								// +0x04
	_STL::vector<Object *> m_objectsCached;							// +0x10
};
// SIZE 0x1C

#endif
