#pragma once
#ifndef BFME2_PRODUCTION_QUEUE_HORDE_CONTAIN_H
#define BFME2_PRODUCTION_QUEUE_HORDE_CONTAIN_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/HordeGarrisonContain.h"

class Thing;
class ModuleData;
class Xfer;

// The leaf uses the complete HordeGarrisonContain layout without additional storage.
typedef char ProductionQueueHordeContain_base_size[(sizeof(HordeGarrisonContain) == 0x9E4) ? 1 : -1];

class ProductionQueueHordeContain : public HordeGarrisonContain
{
public:
	virtual void garrisonSlot29(Object *obj);
	virtual void garrisonSlot30(Object *obj);
	ProductionQueueHordeContain(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;
	virtual ~ProductionQueueHordeContain();

protected:
	virtual void xfer(Xfer *xfer);

	virtual void loadPostProcess() { HordeGarrisonContain::loadPostProcess(); }
};
typedef char ProductionQueueHordeContain_size[(sizeof(ProductionQueueHordeContain) == 0x9E4) ? 1 : -1];

#endif
