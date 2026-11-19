// DeflectSpecialPower.h: the deflect power (an update-module special power that also collides). SIZE 0x40.
// Target facts (retail game.dat 1.06): its constructor 0x004C545D calls SpecialPowerUpdateModule's
// constructor 0x0058959A; the destructor 0x004C54BE tail-calls SpecialPowerUpdateModule's 0x00589079; xfer
// 0x004C5571 is Version(1, 1), SpecialPowerUpdateModule::xfer, an IsLightCRC early-out, then the bytes at +0x3C
// and +0x3D. The CollideModuleInterface base sits at +0x38 (onCollide 0x004C55B2).
#pragma once
#ifndef BFME2_DEFLECTSPECIALPOWER_H
#define BFME2_DEFLECTSPECIALPOWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerUpdateModule.h"
#include "GameLogic/Module/CollideModule.h"

class Thing;
class ModuleData;
class Xfer;

class DeflectSpecialPower : public SpecialPowerUpdateModule, public CollideModuleInterface
{
public:
	DeflectSpecialPower(Thing *thing, const ModuleData *moduleData);	// 0x004C545D
	virtual ~DeflectSpecialPower();										// 0x004C54BE
	virtual NameKeyType getModuleNameKey() const;						// 0x004C54F0
	virtual CollideModuleInterface *getCollide();						// BehaviorModuleInterface slot 1, 0x004C54B1: this as its CollideModule base

protected:
	virtual void xfer(Xfer *xfer);										// 0x004C5571

	Bool m_unk0x3C;		// +0x3C xfer bool
	Bool m_unk0x3D;		// +0x3D xfer bool
protected:
	virtual void loadPostProcess() { SpecialPowerUpdateModule::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialPowerUpdateModule's (h05)
};

#endif
