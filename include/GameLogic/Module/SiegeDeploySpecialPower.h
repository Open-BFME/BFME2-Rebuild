// SiegeDeploySpecialPower.h: the siege engines' deploy power. SIZE 0x74 (ModuleFactory operator new).
// Target facts (retail game.dat 1.06): the constructor 0x004C56BA calls SpecialPowerUpdateModule
// (0x0058959A), stores the tables 0x00C5DCFC at +0x0, 0x00C4E998 at +0xC, 0x00C5DCF0 at +0x10, 0x00C5DCCC at
// +0x20 (SpecialPowerUpdateInterface) and 0x00C5DC68 at +0x24 (SpecialPowerModuleInterface), then zeroes the
// members: ints +0x38/+0x3C/+0x40/+0x44, floats +0x48..+0x5C and +0x64..+0x6C, bytes +0x60 and +0x70.
// xfer 0x004C5B39 is Version(1, 2) after SpecialPowerUpdateModule::xfer and an IsLightCRC early-out: raw int
// +0x38 (the deploy phase), uint +0x3C, ObjectIDs +0x40/+0x44, Coord3D +0x48, bools +0x70 and +0x60, and from
// version 2 the Coord3Ds +0x54 and +0x64. The deploy phase (0 idle, 1 lowering, 2 deployed, 3 raising) is
// read by isActive 0x004C575F (this = the +0x20 subobject, state +0x18 from there: true unless 0 or 1) and by
// stopSpecialPower 0x004C5E41 (this = the +0x24 subobject); setPhase 0x004C5BE3 changes it.
#pragma once
#ifndef BFME2_SIEGEDEPLOYSPECIALPOWER_H
#define BFME2_SIEGEDEPLOYSPECIALPOWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/SpecialPowerUpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

class SiegeDeploySpecialPower : public SpecialPowerUpdateModule
{
public:
	SiegeDeploySpecialPower(Thing *thing, const ModuleData *moduleData);	// 0x004C56BA
	virtual ~SiegeDeploySpecialPower();										// 0x004C57C1

	virtual NameKeyType getModuleNameKey() const;							// 0x004C577C
	virtual UpdateSleepTime update();										// 0x004C5E80 (UpdateModuleInterface slot 0, table 0x0085DCF0)
	virtual bool isActive() const;											// 0x004C575F (SpecialPowerUpdateInterface slot 2)
	virtual bool isReady() const;											// 0x004C5751 (SpecialPowerModuleInterface slot 1, this = the +0x24 subobject): !isActive()
	virtual void specialPowerSlot19() {}										// SpecialPowerModuleInterface slot 19 (table 0x0085DC68): the shared `ret` 0x000B3FD0 (h05 r13)
	virtual unsigned int specialPowerSlot20() const { return m_unk0x40; }			// slot 20: the shared getter 0x0030F2C7 (`mov eax,[ecx+0x1C]` on the +0x24 interface = +0x40)
	virtual void stopSpecialPower();										// 0x004C5E41 (SpecialPowerModuleInterface slot 17)
	void rva004C573C();														// 0x004C573C: tells the object's DynamicPortalBehaviour (the destructor and setPhase call it)
	void rva004C5E62();														// 0x004C5E62: clears status 0x40, returns to phase 0 and forgets the dock

protected:
	virtual const char *GetSnapshotName() const;							// 0x004C5736 (table 0x0085DCFC slot 2)
	virtual void xfer(Xfer *xfer);											// 0x004C5B39

private:
	void setPhase(Int phase);												// 0x004C5BE3

	Int m_phase;				// +0x38 xfer raw int; isActive and stopSpecialPower read it
	UnsignedInt m_unk0x3C;		// +0x3C xfer uint
	ObjectID m_unk0x40;			// +0x40 xfer ObjectID
	ObjectID m_unk0x44;			// +0x44 xfer ObjectID
	Coord3D m_unk0x48;			// +0x48 xfer Coord3D
	Coord3D m_approachPoint;	// +0x54 xfer Coord3D from version 2
	Bool m_unk0x60;				// +0x60 xfer bool (after +0x70)
	Coord3D m_dockPosition;		// +0x64 xfer Coord3D from version 2
	Bool m_stopRequested;		// +0x70 xfer bool; stopSpecialPower sets it in phases 2 and 3
protected:
	virtual void loadPostProcess() { SpecialPowerUpdateModule::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialPowerUpdateModule's (h05)
};
// SIZE 0x74
typedef char SiegeDeploySpecialPower_size[(sizeof(SiegeDeploySpecialPower) == 0x74) ? 1 : -1];

#endif
