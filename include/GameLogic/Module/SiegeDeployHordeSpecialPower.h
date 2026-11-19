// SiegeDeployHordeSpecialPower.h: the horde variant of SiegeDeploySpecialPower. Derives from
// SpecialPowerUpdateModule (constructor 0x004C6386 calls 0x0058959A; destructor 0x004C63EF tail-calls 0x00589079).
// Target facts: xfer 0x004C6416 is SpecialPowerUpdateModule::xfer, an IsLightCRC early-out, Version(1, 2), an ObjectID
// at +0x38, a Coord3D at +0x3C and, from version 2, an int at +0x48. Members past the base are named by offset.
#pragma once
#ifndef BFME2_SIEGEDEPLOYHORDESPECIALPOWER_H
#define BFME2_SIEGEDEPLOYHORDESPECIALPOWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/SpecialPowerUpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

class SiegeDeployHordeSpecialPower : public SpecialPowerUpdateModule
{
public:
	SiegeDeployHordeSpecialPower(Thing *thing, const ModuleData *moduleData);	// 0x004C6386
	virtual ~SiegeDeployHordeSpecialPower();									// 0x004C63EF
	virtual NameKeyType getModuleNameKey() const;								// 0x004C6472
	virtual void setTargetID(ObjectID id);										// slot 13, 0x003B23AF (shared with AIUpdateInterface::setLastCommandSource): stores +0x38
	virtual void setTargetPosition(const Coord3D &position);					// slot 14, 0x004C63DE: stores +0x3C

protected:
	virtual void xfer(Xfer *xfer);												// 0x004C6416

	ObjectID m_unk0x38;		// +0x38 xfer ObjectID
	Coord3D m_unk0x3C;		// +0x3C xfer Coord3D
	Int m_unk0x48;			// +0x48 xfer int from version 2
protected:
	virtual void loadPostProcess() { SpecialPowerUpdateModule::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialPowerUpdateModule's (h05)
};

#endif
