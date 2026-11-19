// WeaponModeSpecialPowerUpdate.h: the weapon-mode special power (an update-module special power, derived
// from SpecialPowerUpdateModule). Target facts: its constructor 0x00494B38 calls SpecialPowerUpdateModule's
// constructor 0x0058959A directly (the only direct caller besides the siege powers); xfer 0x00494BBA
// (the map's `Rva00494A97::xfer`) follows the base. Members past the base are not recovered, so the class
// declares no layout beyond SpecialPowerUpdateModule's 0x38 bytes.
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
	WeaponModeSpecialPowerUpdate(Thing *thing, const ModuleData *moduleData);	// 0x00494B38
	virtual NameKeyType getModuleNameKey() const;								// 0x00494AF3

protected:
	virtual void xfer(Xfer *xfer);												// 0x00494BBA

	Bool m_unk0x38;		// +0x38: the constructor 0x00494B38 clears it, xfer 0x00494BBA transfers it
protected:
	virtual void loadPostProcess() { SpecialPowerUpdateModule::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialPowerUpdateModule's (h05)
};

#endif
