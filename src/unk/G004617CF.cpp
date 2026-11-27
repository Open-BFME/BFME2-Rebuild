// cl: /O1
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004617CF name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Object.h"
#include "GameLogic/Module/BehaviorModule.h"
#include "Common/NameKeyGenerator.h"
// FUNCTION: BFME2 0x004617CF size=108 name=?findFakePathfindPortalBehaviour@@YAPAVBehaviorModule@@PAUFakePortalObjectView@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
BehaviorModule *findFakePathfindPortalBehaviour(Object *object)
{
	static NameKeyType fakePortalKey = TheNameKeyGenerator->nameToKey("FakePathfindPortalBehaviour");
	for (BehaviorModule **module = object->getBehaviorModules(); *module; ++module)
	{
		if ((*module)->getModuleNameKey() == fakePortalKey)
			return *module;
	}
	return NULL;
}
