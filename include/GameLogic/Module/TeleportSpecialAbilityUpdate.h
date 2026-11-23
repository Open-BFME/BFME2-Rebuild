// TeleportSpecialAbilityUpdate: the Teleport special ability. It adds no data to SpecialAbilityUpdate (SIZE 0x88):
// its constructor (0x00492C59) runs the base constructor and installs this class's tables, and its xfer (0x00492E10)
// saves only what the base saves. It overrides the module name key, the snapshot name, triggerAbilityEffect and
// startUnpacking.
#pragma once
#ifndef BFME2_TELEPORT_SPECIAL_ABILITY_UPDATE_H
#define BFME2_TELEPORT_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char TeleportSpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class TeleportSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	TeleportSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// 0x00492D11
	virtual void startUnpacking();	// 0x00492DAB
	virtual bool vslot08(const Coord3D *location);	// SpecialPowerUpdateInterface slot 8, 0x00492E79

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x88
typedef char TeleportSpecialAbilityUpdate_size[(sizeof(TeleportSpecialAbilityUpdate) == 0x88) ? 1 : -1];

#endif
