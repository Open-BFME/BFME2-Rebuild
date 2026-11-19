// ObjectCreationListStore.h: the subsystem that owns every ObjectCreationList and every
// creation nugget. SIZE 0x30.
// Target facts (retail game.dat):
// - ctor 0x001F092A: SubsystemInterface's ctor, table VA 0x00BE100C (14 slots), an STLport
//   map at +0x0C (its default ctor 0x0033C432), vectors at +0x18 and +0x24.
// - dtor 0x001F0996 deletes every nugget in +0x24 and clears it, deletes every list held by
//   the map (node value +0x14) and clears it, deletes every list in +0x18 by index and
//   clears it.
// - findObjectCreationList 0x001F07B8 keys the map by NameKeyGenerator::nameToKey and
//   returns NULL for "None". parseObjectCreationListDefinition 0x001F0C0B moves a list a
//   reload replaces (load type 5) from the map to +0x18 and sets its retired flag.
// - Slots over SubsystemInterface's: 0 dtor (??_G 0x001F0BEF), 1 init, 9 reset and 10
//   update are empty (fold 0x000B3FD0), 4 attemptReload 0x001F04D9.
// - TheObjectCreationListStore is VA 0x00DFDCCC.
#pragma once
#ifndef BFME2_OBJECTCREATIONLISTSTORE_H
#define BFME2_OBJECTCREATIONLISTSTORE_H

#include <stddef.h>
#include <map>
#include <vector>
#include "Common/SubsystemInterface.h"
#include "Common/NameKeyGenerator.h"
#include "GameLogic/ObjectCreationList.h"

class INI;

class ObjectCreationListStore : public SubsystemInterface
{
public:
	ObjectCreationListStore();												// 0x001F092A
	virtual ~ObjectCreationListStore();										// 0x001F0996

	virtual void init() {}
	virtual Bool attemptReload(Bool *needsRestart);							// 0x001F04D9
	virtual void reset() {}
	virtual void update() {}

	const ObjectCreationList *findObjectCreationList(const char *name) const;	// 0x001F07B8
	static void parseObjectCreationListDefinition(INI *ini);					// 0x001F0C0B
	void addObjectCreationNugget(ObjectCreationNugget *nugget);				// 0x001F0AC4

	std::map<NameKeyType, ObjectCreationList *> m_lists;					// 0x0C
	std::vector<ObjectCreationList *> m_retiredLists;						// 0x18
	std::vector<ObjectCreationNugget *> m_allNuggets;						// 0x24
};
typedef char ObjectCreationListStore_size[(sizeof(ObjectCreationListStore) == 0x30) ? 1 : -1];
typedef char ObjectCreationListStore_off_m_lists[(offsetof(ObjectCreationListStore, m_lists) == 0x0C) ? 1 : -1];
typedef char ObjectCreationListStore_off_m_retiredLists[(offsetof(ObjectCreationListStore, m_retiredLists) == 0x18) ? 1 : -1];
typedef char ObjectCreationListStore_off_m_allNuggets[(offsetof(ObjectCreationListStore, m_allNuggets) == 0x24) ? 1 : -1];

extern ObjectCreationListStore *TheObjectCreationListStore;

#endif
