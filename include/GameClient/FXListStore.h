// FXListStore.h: the subsystem that owns every FXList. SIZE 0x2C.
// Target facts (retail game.dat):
// - Table VA 0x00BDD97C (14 slots over SubsystemInterface's): 0 dtor (??_G 0x001E2C92), 1 init,
//   9 reset, 10 update empty (fold 0x000B3FD0), 4 attemptReload 0x001E0DB9.
// - +0x0C is a hash_map keyed by NameKeyType: findFXList 0x001E281A and
//   parseFXListDefinition 0x001E2CAE call its _M_find 0x002888D4 and read the node's value
//   at +0x08; operator[] 0x0041F4E5 and erase(iterator) 0x001E2861 are the map's
//   `hash_map<NameKeyType, Rva001E2747 *, rts::hash<NameKeyType>, equal_to<NameKeyType>>`.
// - +0x20 is the vector of lists a reload replaced (push_back 0x004DFCB0).
// - TheFXListStore is VA 0x00DFDC3C.
#pragma once
#ifndef BFME2_FXLISTSTORE_H
#define BFME2_FXLISTSTORE_H

#include <vector>
#include "Common/SubsystemInterface.h"
#include "Common/NameKeyGenerator.h"
#include "Common/STLTypedefs.h"
#include "GameLogic/FXList.h"

class INI;

class FXListStore : public SubsystemInterface
{
public:
	FXListStore();
	virtual ~FXListStore();

	virtual void init() {}
	virtual Bool attemptReload(Bool *needsRestart);										// 0x001E0DB9
	virtual void reset() {}
	virtual void update() {}

	const FXList *findFXList(const char *name) const;									// 0x001E281A
	static void parseFXListDefinition(INI *ini);										// 0x001E2CAE

	_STL::hash_map<NameKeyType, FXList *, rts::hash<NameKeyType>, _STL::equal_to<NameKeyType> > m_fxmap;	// 0x0C
	std::vector<FXList *> m_retiredLists;												// 0x20
};
typedef char FXListStore_size[(sizeof(FXListStore) == 0x2C) ? 1 : -1];

extern FXListStore *TheFXListStore;

#endif
