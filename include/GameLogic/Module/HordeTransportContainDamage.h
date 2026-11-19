// HordeTransportContainDamage.h: the HordeTransportContainDamage module. SIZE 0x14 (ModuleFactory operator new).
// The module handles damage directly. Its healing and body-state callbacks are empty.
#pragma once
#ifndef BFME2_HORDE_TRANSPORT_CONTAIN_DAMAGE_H
#define BFME2_HORDE_TRANSPORT_CONTAIN_DAMAGE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DamageModule.h"

class Thing;
class ModuleData;
class Xfer;

// DamageModule ends at 0x14; this class's members follow it.
typedef char HordeTransportContainDamage_base_size[(sizeof(DamageModule) == 0x14) ? 1 : -1];

class DamageInfo;
class HordeTransportContainDamage : public DamageModule
{
public:
	HordeTransportContainDamage(Thing *thing, const ModuleData *moduleData);
	virtual ~HordeTransportContainDamage();
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	virtual void onDamage(DamageInfo *damageInfo);
	virtual void onHealing(DamageInfo *) {}
	virtual void onBodyDamageStateChange(const DamageInfo *, BodyDamageType, BodyDamageType) {}

protected:
	virtual void loadPostProcess() { DamageModule::loadPostProcess(); }
};
// SIZE 0x14
typedef char HordeTransportContainDamage_size[(sizeof(HordeTransportContainDamage) == 0x14) ? 1 : -1];

#endif
