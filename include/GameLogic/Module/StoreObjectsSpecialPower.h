#pragma once
#include <vector>
#include "GameLogic/Module/SpecialAbilityUpdate.h"
class StoreObjectsSpecialPower : public SpecialAbilityUpdate
{
public:
 void rva004CDA35();
 _STL::vector<ObjectID> m_storedObjects; // 0x88
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
