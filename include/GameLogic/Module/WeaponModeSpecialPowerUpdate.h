// WeaponModeSpecialPowerUpdate.h: the weapon-mode special power (an update-module special power, derived
// from SpecialPowerUpdateModule). Target facts: its constructor 0x00494B38 calls SpecialPowerUpdateModule's
// constructor 0x0058959A directly (the only direct caller besides the siege powers); xfer 0x00494BBA
// also transfers the active flag. The update ends that mode when its wake
// frame arrives, then sleeps until the next activation.
#pragma once
#ifndef BFME2_WEAPONMODESPECIALPOWERUPDATE_H
#define BFME2_WEAPONMODESPECIALPOWERUPDATE_H

#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerUpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

class WeaponModeSpecialPowerUpdate : public SpecialPowerUpdateModule
{
public:
	WeaponModeSpecialPowerUpdate(Thing *thing, const ModuleData *moduleData);
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);	// 0x0024DCE9
	virtual NameKeyType getModuleNameKey() const;
	virtual bool isActive() const { return m_active; }
	virtual UpdateSleepTime update();								// 0x00494AF3

protected:
	virtual void xfer(Xfer *xfer);												// 0x00494BBA

	Bool m_active;		// +0x38: the constructor 0x00494B38 clears it, xfer 0x00494BBA transfers it
protected:
	virtual void loadPostProcess() { SpecialPowerUpdateModule::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialPowerUpdateModule's
};

#endif
