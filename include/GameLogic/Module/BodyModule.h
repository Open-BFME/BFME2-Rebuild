// BodyModule.h: SAGE BodyModuleInterface (43 slots) and the concrete BodyModule.
// Target facts (old repo Code/GameEngine/Include/GameLogic/Module/BodyModule.h):
// Object::getBodyModule returns this from Object+0x254. Slot 15 is
// getLastDamageInfo (BoneFXUpdate 0x0048740D reads the damage type at +0x10 of
// the result). Unrecovered slots keep their address names.
#pragma once
#ifndef BFME2_BODYMODULE_H
#define BFME2_BODYMODULE_H

#include "Common/GameType.h"
#include "GameLogic/Module/BehaviorModule.h"

class AsciiString;
class BodyModule;
class Object;
struct BfmeBodyDataPair8;
class Coord2D;
struct DamageInfo;
struct DamageInfoInput;
template <int N> class BitFlags;
enum ArmorSetType;
// The names are the INI table at VA 0x00DCCD14 (ChangeType of MaxHealthUpgrade), in its order.
enum MaxHealthChangeType
{
	SAME_CURRENTHEALTH = 0,
	PRESERVE_RATIO = 1,
	ADD_CURRENT_HEALTH_TOO = 2
};
enum VeterancyLevel;

class BodyModuleInterface
{
public:
	virtual void attemptDamage(DamageInfo *) = 0; // 0
	virtual void attemptHealing(DamageInfo *) = 0; // 1
	virtual Real estimateDamage(DamageInfoInput &) const = 0; // 2
	virtual void reapplyDamageState() = 0; // 3
	virtual Real getHealth() const = 0; // 4
	virtual Real getHealthRatio() const = 0; // 5
	virtual Real getMaxHealth() const = 0; // 6
	virtual Real getInitialHealth() const = 0; // 7
	virtual BodyDamageType getDamageState() const = 0; // 8
	virtual void setDamageState(BodyDamageType) = 0; // 9
	virtual void updateBodyParticleSystems() = 0; // 10
	virtual void onVeterancyLevelChanged(VeterancyLevel, VeterancyLevel) = 0; // 11
	virtual void setArmorSetFlag(ArmorSetType) = 0; // 12
	virtual void clearArmorSetFlag(ArmorSetType) = 0; // 13
	virtual BitFlags<21> getArmorSetFlags() = 0; // 14
	virtual const DamageInfo *getLastDamageInfo() const = 0; // 15
	virtual unsigned int getLastDamageTimestamp() const = 0; // 16
	virtual unsigned int getLastHealingTimestamp() const = 0; // 17
	virtual ObjectID getAndClearLastAttacker() = 0; // 18
	virtual unsigned char getFrontCrushed() const = 0; // 19
	virtual Bool getBackCrushed() const = 0; // 20
	virtual void setInitialHealth(Real, Bool) = 0; // 21
	virtual void setMaxHealth(Real, MaxHealthChangeType) = 0; // 22
	virtual void rva004BDB37() = 0; // 23
	virtual void setFrontCrushed(unsigned char) = 0; // 24
	virtual void setBackCrushed(unsigned char) = 0; // 25
	virtual void applyDamageScalar(Real) = 0; // 26
	virtual Real getDamageScalar() const = 0; // 27
	virtual AsciiString getGrabObjectName() const = 0; // 28
	virtual int getGrabFX() const = 0; // 29
	virtual Real getGrabDamage() const = 0; // 30
	virtual Coord2D getGrabOffset() = 0; // 31: a non-trivial Coord2D by value (hidden result pointer; ActiveBody 0x004BDC56, InactiveBody 0x004BD8A2)
	virtual void internalChangeHealth(Real, DamageInfo *) = 0; // 32
	virtual void setIndestructible(Bool) = 0; // 33
	virtual unsigned char isIndestructible() const = 0; // 34
	virtual Real getDodgePercent() const = 0; // 35
	virtual void rvaSlot36() = 0; // 36
	virtual void rva004C15F7(unsigned char) = 0; // 37
	virtual Bool rva004C2127(Object *) = 0; // 38
	virtual void rvaSlot39(BodyDamageType) = 0; // 39
	virtual void rvaSlot40(Real, Bool) = 0; // 40
	virtual BodyModule *getIface() = 0; // 41
	virtual void setHealth(Real) = 0; // 42
};

// BodyModule: BehaviorModule (ctor 0x00253330) plus the interface at +0x10 and
// m_damageScalar at +0x14, set to 1.0f; size 0x18. Ctor 0x004BD7FD stores
// 0x00C5AE68 at +0, 0x00C5AD78 at +0xC and 0x00C5AC18 at +0x10; ~BodyModule
// 0x004BD763 restores BehaviorModule's tables and jumps to ~Module.
// Its table 0x00C5AC18 fills 28 of the 43 interface slots with the default
// bodies below (folded stubs: 0x002BFBF0 returns 0.0f, 0x0036CC7A returns -1,
// 0x000D43D0 returns 0, 0x0047A699 / 0x0050B5C6 return false / true, the rest
// are empty with their argument counts); applyDamageScalar 0x004BD77C multiplies
// and getDamageScalar 0x00452D82 returns m_damageScalar.
class BodyModule : public BehaviorModule, public BodyModuleInterface
{
public:
	BodyModule(Thing *thing, const ModuleData *moduleData);				// 0x004BD7FD
	virtual ~BodyModule();												// 0x004BD763

	virtual BodyModuleInterface *getBody() { return this; }
	virtual Bool rva0050B5C6() { return true; }							// primary slot 12 (0x0050B5C6, `mov al,1; ret`); BodyModule's table has 13 slots (f14, provisional role name: the map folds it)

	virtual Real getMaxHealth() const { return 0.0f; }					// 6
	virtual Real getInitialHealth() const { return 0.0f; }					// 7
	virtual const DamageInfo *getLastDamageInfo() const { return 0; }	// 15
	virtual unsigned int getLastDamageTimestamp() const { return 0xFFFFFFFF; }	// 16
	virtual unsigned int getLastHealingTimestamp() const { return 0xFFFFFFFF; }	// 17
	virtual ObjectID getAndClearLastAttacker() { return (ObjectID)0; }				// 18
	virtual unsigned char getFrontCrushed() const { return 0; }				// 19
	virtual Bool getBackCrushed() const { return false; }				// 20
	virtual void setInitialHealth(Real, Bool) {}						// 21
	virtual void setMaxHealth(Real, MaxHealthChangeType) {}				// 22
	virtual void rva004BDB37() {}										// 23
	virtual void setFrontCrushed(unsigned char) {}							// 24
	virtual void setBackCrushed(unsigned char) {}							// 25
	virtual void applyDamageScalar(Real scalar) { m_damageScalar *= scalar; }	// 26
	virtual Real getDamageScalar() const { return m_damageScalar; }		// 27
	virtual void setIndestructible(Bool) {}								// 33
	virtual unsigned char isIndestructible() const { return 1; }				// 34
	virtual Real getDodgePercent() const { return 0.0f; }					// 35
	virtual void rvaSlot36() {}											// 36
	virtual void rva004C15F7(unsigned char) {}							// 37
	virtual Bool rva004C2127(Object *) { return true; }					// 38
	virtual void rvaSlot39(BodyDamageType) {}							// 39
	virtual void rvaSlot40(Real, Bool) {}								// 40
	virtual BodyModule *getIface() { return 0; }						// 41
	virtual void setHealth(Real) {}									// 42

protected:
	virtual const char *GetSnapshotName() const;						// Snapshot slot 2, 0x004BD83B ("BodyModule", table 0x0085AE68)
	virtual void xfer(Xfer *xfer);							// Snapshot slot 3, 0x0058B043
	Real m_damageScalar;												// 0x14
protected:
	virtual void loadPostProcess();	// out of line (a jmp to BehaviorModule's), ICF-folded with UpdateModule's at 0x0058B03E; inline, MSVC folds it into each subclass's forwarder and loses retail's second level (0x00497805) (h05)
};
// SIZE 0x18

#endif
