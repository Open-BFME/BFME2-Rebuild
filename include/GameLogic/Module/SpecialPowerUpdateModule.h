// SpecialPowerUpdateModule.h: the base of the update-module special powers (SiegeDeploySpecialPower,
// SiegeDeployHordeSpecialPower, DeflectSpecialPower, WeaponModeSpecialPowerUpdate): an UpdateModule that
// also answers the special-power module interface, with the pause bookkeeping SpecialPowerModule keeps.
// SIZE 0x38.
//
// Target facts (retail game.dat 1.06):
// - The name is the binary's own: the primary table's slot 2 (0x005890A0) returns the string at VA 0x00C701C4,
//   "SpecialPowerUpdateModule" (every module's snapshot name works this way), and the module data's field table
//   at 0x008701E0 (buildFieldParse 0x005890ED) belongs to SpecialPowerUpdateModuleData. The map spells this class
//   Rva00589079 (destructor chain 0x00589079, deleting destructor 0x005890FE, xfer 0x00589215) and
//   WeaponModeSpecialPowerUpdateBase (constructor 0x0058959A); they are one class.
// - Constructor 0x0058959A: UpdateModule(thing, data) (0x00253390), then 0x00C6F248 at +0x20, 0x00C4E2B0 at +0x24
//   (the interface bases' tables), then its own tables 0x00C70190 at +0x0, 0x00C4E998 at +0xC, 0x00BFBCE4 at +0x10,
//   0x0087016C-table at +0x20 (9 slots, SpecialPowerUpdateInterface) and 0x00870108 at +0x24 (25 slots,
//   SpecialPowerModuleInterface). The destructor 0x00589079 resets the same five and tail-jumps to UpdateModule's.
// - Members after the interfaces follow xfer 0x00589215 (Version 1, UpdateModule::xfer, then a uint at +0x28, an
//   int at +0x2C, a uint at +0x30 and a float at +0x34) and the pause code 0x005890A6 (interface +0x24 this:
//   count +0x2C, frame +0x30, percent +0x34), the same four fields SpecialPowerModule keeps at +0x18..+0x24.
// - The interface overrides take the +0x24 subobject as `this` (module data = this-0x20, object = this-0x1C).
//   Their bodies are the placeholder-class rows of 0x00589079's neighbours (0x0058911A, 0x00589297, 0x005892F6,
//   0x00589414, 0x00589560, ...) and are not written yet; only the declarations that make the table true are here.
#pragma once
#ifndef BFME2_SPECIALPOWERUPDATEMODULE_H
#define BFME2_SPECIALPOWERUPDATEMODULE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Rva002390CB.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/SpecialPowerUpdateInterface.h"
#include "GameLogic/Module/SpecialPowerModule.h"

class MultiIniFieldParse;
class SpecialPowerTemplate;
class Thing;
class Xfer;

class SpecialPowerUpdateModuleData : public ModuleData
{
public:
	static void buildFieldParse(MultiIniFieldParse &parse);		// 0x005890ED (the map's Rva005890EDBase)

	const SpecialPowerTemplate *m_specialPowerTemplate;			// 0x08 SpecialPowerTemplate (the interface's template getter reads data+0x8)
	Rva002390CB m_initiateSound;								// 0x0C InitiateSound (getInitiateSound 0x00589560 copies data+0xC)
	Bool m_startsPaused;										// 0x14 StartsPaused
};
// SIZE 0x18

typedef char SpecialPowerUpdateModule_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class SpecialPowerUpdateModule : public UpdateModule, public SpecialPowerUpdateInterface, public SpecialPowerModuleInterface
{
public:
	SpecialPowerUpdateModule(Thing *thing, const ModuleData *moduleData);	// 0x0058959A

	// BehaviorModuleInterface slots 8 and 25 expose the two interfaces at +0x24 and +0x20.
	virtual SpecialPowerModuleInterface *getSpecialPower() { return this; }
	virtual SpecialPowerUpdateInterface *getSpecialPowerUpdateInterface() { return this; }
	virtual ~SpecialPowerUpdateModule();									// 0x005890FE scalar deleting, chain 0x00589079

	// SpecialPowerUpdateInterface (+0x20): table 0x0087016C. isActive stays pure (slot 2 is __purecall there).
	virtual void initiateIntentToDoSpecialPower(const SpecialPowerTemplate *specialPowerTemplate,
		const Object *targetObj, const Coord3D *targetPos, unsigned int commandOptions,
		const Waypoint *way);											// slot 0, shared stub 0x0025EF04
	virtual bool isSpecialAbility() const { return false; }			// slot 1, shared 0x0047A699 (false)
	virtual bool isPowerCurrentlyInUse(const CommandButton *command = 0) const;	// slot 6, 0x00494ABE: isActive()
	virtual bool vslot07(int power);									// slot 7, 0x00494ADF: whether the module data names this power
	virtual bool vslot08(int) { return true; }							// slot 8, shared `mov al,1; ret 4` 0x005CB9FA
	virtual CommandOption getCommandOption() const { return (CommandOption)0; }	// slot 3, shared `xor eax,eax; ret` 0x000D43D0
	virtual bool doesSpecialPowerHaveOverridableDestinationActive() const { return false; }	// slot 4, shared 0x0047A699
	virtual void setSpecialPowerOverridableDestination(const Coord3D *loc) {}	// slot 5, shared `ret 4` 0x0047A69C

	// SpecialPowerModuleInterface (+0x24): table 0x00870108.
	virtual bool isModuleForPower(const SpecialPowerTemplate *power) const;		// 0x0058911A
	virtual bool isReady() const;												// 0x00589297
	virtual float getPercentReady() const;										// 0x005892F6
	virtual unsigned int getReadyFrame() const;									// 0x005893AD
	virtual AsciiString getPowerName() const;									// 0x00589414
	virtual const SpecialPowerTemplate *getSpecialPowerTemplate() const;		// 0x0058912E
	virtual void pauseCountdown(bool pause);									// 0x005890A6
	virtual void doSpecialPower(unsigned int commandOptions);					// 0x00589135
	virtual void doSpecialPowerAtObject(Object *object, unsigned int commandOptions);	// 0x0058916B
	virtual void doSpecialPowerAtLocation(const Coord3D *location, unsigned int commandOptions);	// 0x005891A3
	virtual void doSpecialPowerUsingWaypoints(const Coord3D *location, const Waypoint *waypoint, unsigned int commandOptions);	// 0x005891DB
	// Behavior slot 43 forwards the float to the special-power interface at +0x24.
	virtual void slot43(float readyFraction);
	virtual void startPowerRecharge(float readyFraction);						// 0x0058943A
	virtual Rva002390CB getInitiateSound() const;								// 0x00589560
	// The rest of table 0x00870108: shared bodies (h05 r13, retail slot targets).
	virtual bool specialPowerSlot03() const { return m_pausedCount > 0; }		// slot 3, 0x005778AD (`cmp [ecx+8],eax; setg al` on the +0x24 interface)
	virtual void onSpecialPowerCreation() {}									// slot 7, the shared `ret` 0x000B3FD0
	virtual void setReadyFrame(unsigned int frame) { m_unk0x28 = frame; }		// slot 8, the shared setter 0x002B230F (`mov [ecx+4],eax; ret 4`)
	virtual void markSpecialPowerTriggered(const Coord3D *location) {}			// slot 14, the shared `ret 4` 0x0047A69C
	virtual void stopSpecialPower() {}											// slot 17, 0x000B3FD0
	virtual bool rva0049466D(const Coord3D *location) { return true; }			// slot 18, the shared `mov al,1; ret 4` 0x005CB9FA
	virtual bool specialPowerSlot21() const { return false; }					// slot 21, 0x0047A699
	virtual void specialPowerSlot22() {}										// slot 22, 0x000B3FD0
	virtual SpecialPowerModule *specialPowerSlot23() { return 0; }					// slot 23, the shared `xor eax,eax; ret` 0x000D43D0
	virtual void *specialPowerSlot24() { return 0; }							// slot 24, 0x000D43D0

protected:
	virtual const char *GetSnapshotName() const;								// slot 2, 0x005890A0 (returns "SpecialPowerUpdateModule")
	virtual void xfer(Xfer *xfer);												// 0x00589215

	UnsignedInt m_unk0x28;		// +0x28 (xfer: unsigned int)
	Int m_pausedCount;			// +0x2C (pauseCountdown 0x005890A6)
	UnsignedInt m_pausedOnFrame;	// +0x30
	Real m_pausedPercent;		// +0x34
protected:
	virtual void loadPostProcess();	// 0x00497805, a jmp to UpdateModule's; out of line, so its subclasses' own forwarders (0x00483CEF) jump here (h05)
};
// SIZE 0x38

#endif
