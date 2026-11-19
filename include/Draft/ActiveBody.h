// DRAFT (h05 modgen, promoted in part by f14): generated from constructor RVA 0x004BF6A1 and xfer RVA 0x004BF1E8.
// ActiveBody.h: the ActiveBody module. SIZE 0x100 (ModuleFactory operator new).
// Target facts: the constructor 0x004BF6A1 calls BodyModule first; it stores the tables 0x00C5B038
// at +0x0 (23 slots), 0x00C5AD78 at +0xC and 0x00C5B5B0 at +0x10 (the BodyModuleInterface, 43 slots).
// Members are what the constructor (0x004BF6A1) initializes and what xfer (0x004BF1E8) transfers after
// BodyModule::xfer, in that order; the names are role names (provisional) taken from how the rows use
// them: +0x18 is the value getHealthRatio divides by +0x20, internalChangeHealth moves +0x18 to +0x1C
// before adding, setDamageState turns +0x24/+0x28 (ratios of +0x20) into health targets.
//
// Primary table (+0x0), slots after the Snapshot/Module/ObjectModule/BehaviorModule ones (0..12):
//   0 ~ActiveBody 0x004BF951 (via the deleting thunk 0x004BF9D1)  1 loadPostProcess 0x004BF40F
//   2 GetSnapshotName 0x004BF88D  3 xfer 0x004BF1E8  4 getModuleNameKey 0x004BF848  8 onDelete 0x004BDCAC
//   13 applyDamageStateAttributeModifiers  14 0x004BF8AC (thunk)  15 0x004BF8DF  16 0x004BF8ED  17 validateArmorAndDamageFX 0x004BE1AE
//   18 doDamageFX  19 createParticleSystems  20 0x004BE3EF  21 setCorrectDamageState  22 0x004BF9ED
// Interface table (+0x10, BodyModuleInterface): the overrides are declared below next to their rows.
#pragma once
#ifndef BFME2_ACTIVE_BODY_H
#define BFME2_ACTIVE_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"
#include "Common/Coord3D.h"
#include "Draft/ActiveBodyModuleData.h"
#include "GameLogic/Armor.h"
#include "GameLogic/Damage.h"
#include "GameLogic/Module/BodyModule.h"

class Thing;
class Object;
class ModuleData;
class Xfer;
class DamageFX;
class FXList;
class ObjectCreationList;
class ParticleSystemTemplate;
enum ArmorSetType;

// A node of the particle-system list at +0xC8 (12 bytes: table 0x00C5AEB0, the particle system's id,
// the next node); createParticleSystems pushes at the head, stopClientParticleSystems 0x004BE3EF pops.
struct ActiveBodyParticleNode
{
	virtual ~ActiveBodyParticleNode() {}	// table 0x00C5AEB0 (its slot 0 is what stopClientParticleSystems calls with flag 0)
	ParticleSystemID m_systemID;			// +0x4
	ActiveBodyParticleNode *m_next;			// +0x8
};

// BodyModule ends at 0x18; this class's members follow it.
typedef char ActiveBody_base_size[(sizeof(BodyModule) == 0x18) ? 1 : -1];

class ActiveBody : public BodyModule
{
public:
	ActiveBody(Thing *thing, const ModuleData *moduleData);		// 0x004BF6A1
	virtual ~ActiveBody();										// slot 0, 0x004BF951
	virtual NameKeyType getModuleNameKey() const;				// slot 4, 0x004BF848
	virtual void onDelete();									// slot 8, 0x004BDCAC

	// Primary slots 13..22 are ActiveBody's own virtuals, in retail order.
	virtual void applyDamageStateAttributeModifiers();									// 13: applies the damaged / really damaged attribute modifier of the module data (+0x30 / +0x34)
	virtual int rva004BF8AC();									// 14: thunk 0x004BF8AC, jumps to Armor::rva001D9083 on the armor at +0xF8 (map: Rva004BF8AC)
	virtual ObjectID getClearableLastAttacker() const;			// 15: 0x004BF8DF, 0 once the +0xC6 flag is set, else the last damage source id at +0x48 (Zero Hour's name)
	virtual void clearLastAttacker();							// 16: 0x004BF8ED, sets the +0xC6 flag (Zero Hour's name)
	virtual void validateArmorAndDamageFX() const;				// 17: 0x004BE1AE (primary slot 17: estimateDamage 0x004BDB01 and attemptDamage 0x004BFE07 reach it as `call [eax+0x44]`; publish 3a accepts the UBE spelling)
protected:
	virtual void doDamageFX(const DamageInfo *damageInfo);		// 18: 0x004BED62 (MAE)
	virtual void createParticleSystems(const AsciiString &boneBaseName, const ParticleSystemTemplate *systemTemplate, Int maxSystems);	// 19: 0x004BE24A (MAE)
public:
	virtual void deleteAllParticleSystems();					// 20: 0x004BE3EF, pops the particle list at +0xC8 (Zero Hour's name; the ledger spells it W3DModelDraw::stopClientParticleSystems, a wrong class: waits on the map)
	virtual void setCorrectDamageState(Bool forceUpdate);		// 21: 0x004BE8BD
	virtual void rva004BF9ED();									// 22: 0x004BF9ED, called by onDelete

	// BodyModuleInterface overrides (the +0x10 table), by slot.
	virtual void attemptDamage(DamageInfo *damageInfo);					// 0, 0x004BFE07
	virtual void attemptHealing(DamageInfo *damageInfo);				// 1, 0x004BEE21
	virtual void reapplyDamageState();											// 3, 0x004BDB25 (18 B): setDamageState(getDamageState()); the map files it under the placeholder class Rva0020DFC0Object
	virtual Real getHealth() const;										// 4, the folded getter 0x004F5F92
	virtual Real getHealthRatio() const;								// 5, 0x004BDC25 (the map's Rva0020E150Value::positiveRatio)
	virtual Real getMaxHealth() const;									// 6, the folded getter 0x005D870D
	virtual Real getInitialHealth() const;									// 7, the folded getter 0x00094A94: the initial health
	virtual BodyDamageType getDamageState() const;						// 8
	virtual void updateBodyParticleSystems();							// 10, 0x004BDCE2 (745 B; the map's Rva0020FE20ActiveBodyInterface); not written yet
	virtual const DamageInfo *getLastDamageInfo() const;				// 15, the folded `lea eax,[ecx+0x30]` 0x00314046
	virtual unsigned int getLastDamageTimestamp() const;				// 16, the folded getter 0x0023CAD2
	virtual unsigned int getLastHealingTimestamp() const;				// 17, the folded getter 0x005BE1A7
	virtual unsigned char getFrontCrushed() const;							// 19, the folded byte getter: m_frontCrushed
	virtual void rva004BDB37();											// 23, 0x004BDB37 (75 B): re-evaluates the visual damage state three times; not written yet
	virtual void setFrontCrushed(unsigned char value);						// 24, 0x004BF8BE: sets m_frontCrushed
	virtual void setBackCrushed(unsigned char value);						// 25, 0x004BF8CB: sets m_backCrushed
	virtual int getGrabFX() const;									// 29, 0x004BDC48: the module data's +0x38 word (GrabFX)
	virtual Real getGrabDamage() const;									// 30, 0x004BDC4F: the module data's +0x3C (GrabDamage)
	virtual unsigned char isIndestructible() const;							// 34, 0x004BF8D8: m_indestructible
	virtual Real getDodgePercent() const;									// 35, 0x004BDC41: the module data's +0x18 (DodgePercent)
	virtual void rvaSlot36();											// 36, 0x004BDC03: Object::rva0028FCD9(state, side), then updateBodyParticleSystems(); not written yet
	virtual BodyModule *getIface();										// 41, 0x004BF893: this - 0x10
	virtual void setHealth(Real health);								// 42, the folded setter 0x00101CBE: m_currentHealth = health
	virtual BitFlags<21> getArmorSetFlags();									// 14, 0x004BF8F5: the armor set flags by value; not written yet
	virtual AsciiString getGrabObjectName() const;							// 28, 0x004BE67E: the module data's GrabObject name; not written yet
	virtual Coord2D getGrabOffset();									// 31, 0x004BDC56: the module data's GrabOffset (+0x40)
	virtual Real estimateDamage(DamageInfoInput &damageInfo) const;	// 2, 0x004BDB01
	virtual void setDamageState(BodyDamageType state);					// 9, 0x004BDAA9
	virtual void onVeterancyLevelChanged(VeterancyLevel oldLevel, VeterancyLevel newLevel);	// 11, 0x004BE47D
	virtual void setArmorSetFlag(ArmorSetType type);					// 12, 0x004BF911
	virtual void clearArmorSetFlag(ArmorSetType type);					// 13, 0x004BF930
	virtual ObjectID getAndClearLastAttacker();										// 18: returns the id at +0x48 once, then 0
	virtual Bool getBackCrushed() const;								// 20, 0x004BF8B7
	virtual void setInitialHealth(Real initialHealth, Bool flag);		// 21, 0x004C0642
	virtual void setMaxHealth(Real maxHealth, MaxHealthChangeType change);	// 22, 0x004BDB85
	virtual void internalChangeHealth(Real amount, DamageInfo *damageInfo);	// 32, 0x004BF005
	virtual void setIndestructible(Bool indestructible);				// 33, 0x004BF186

protected:
	virtual void xfer(Xfer *xfer);								// slot 3, 0x004BF1E8
	virtual void loadPostProcess();								// slot 1, 0x004BF40F: the base, then the "Bookend" shape check of a status-0x53 object
	static Bool shouldRetaliate(Object *obj);					// 0x004BE056

public:
	Int calcDamageState() const;								// 0x004BDA29 (no row of its own): the damage state implied by the health
	void handleStructureReducedToRubble();						// 0x004BDA67 (no row of its own): the owner's bookkeeping when a structure reaches rubble
	void rva004BFB1C(Real amount, const Coord3D *position);	// 0x004BFB1C (QAE): the side-damage split from a position (not written yet)
	void rva004BFCD4(Real amount, DamageInfo *damageInfo);	// 0x004BFCD4 (QAE): attemptDamage's directional branch: the damage source's position, then 0x004BFB1C
	const char *name() const;					// 0x004BF88D (non-virtual, the literal)
	virtual const char *GetSnapshotName() const;

	Real m_currentHealth;						// +0x18 xfer Real
	Real m_prevHealth;							// +0x1C xfer Real; the health before the last change
	Real m_maxHealth;							// +0x20 xfer Real
	Real m_damagedRatio;						// +0x24 xfer Real; MaxHealthDamaged / max (module data +0x10)
	Real m_reallyDamagedRatio;					// +0x28 xfer Real; MaxHealthReallyDamaged / max (module data +0x14)
	Real m_initialHealth;						// +0x2C xfer Real
	Int m_curDamageState;						// +0x30 BodyDamageType; xfer label "BodyDamageType" (0x00305E12)
	Int m_bodySideDestroyed;					// +0x34 xfer label "BodySideDestroyedType" (0x00305E2A); 0..3, written by setCorrectDamageState
	UnsignedInt m_nextDamageFXTime;				// +0x38 xfer UnsignedInt
	Int m_lastDamageFXType;						// +0x3C xfer DamageFXType (0x00305EBA); starts at 29 (undefined)
	DamageInfo m_lastDamageInfo;				// +0x40 (0x7C); the constructor 0x00263895, xfer slot 0 on it
	UnsignedInt m_lastDamageTimestamp;			// +0xBC xfer UnsignedInt
	UnsignedInt m_lastHealingTimestamp;			// +0xC0 xfer UnsignedInt; attemptHealing stores the frame
	Bool m_frontCrushed;						// +0xC4 xfer Bool
	Bool m_backCrushed;							// +0xC5 xfer Bool
	Bool m_lastDamageCleared;					// +0xC6 xfer Bool; set by getAndClearLastAttacker / clearLastAttacker
	Bool m_indestructible;						// +0xC7 xfer Bool; written by setIndestructible
	ActiveBodyParticleNode *m_particleSystems;	// +0xC8 list head; xfer loop (count, then ParticleSystemIDs)
	Real m_damageStateValues[4];				// +0xCC xfer Real each
	Bool m_damageStateFlags[4];			// +0xDC xfer Bool each
	std::vector<ActiveBodyDamageEffect> m_damageEffects;	// +0xE0 copied from the module data +0x58 by the constructor
	ObjectID m_unk0xEC;							// +0xEC xfer ObjectID
	BitFlags<21> m_armorSetFlags;				// +0xF0 xfer through 0x004BE781; setArmorSetFlag / clearArmorSetFlag
	mutable const ArmorTemplateSet *m_curArmorSet;	// +0xF4 the armor set found by validateArmorAndDamageFX
	mutable Armor m_curArmor;						// +0xF8 (the armor's template name)
	mutable DamageFX *m_curDamageFX;				// +0xFC
};
// SIZE 0x100
typedef char ActiveBody_size[(sizeof(ActiveBody) == 0x100) ? 1 : -1];
typedef char ActiveBody_off_m_currentHealth[(offsetof(ActiveBody, m_currentHealth) == 0x18) ? 1 : -1];
typedef char ActiveBody_off_m_initialHealth[(offsetof(ActiveBody, m_initialHealth) == 0x2C) ? 1 : -1];
typedef char ActiveBody_off_m_curDamageState[(offsetof(ActiveBody, m_curDamageState) == 0x30) ? 1 : -1];
typedef char ActiveBody_off_m_lastDamageInfo[(offsetof(ActiveBody, m_lastDamageInfo) == 0x40) ? 1 : -1];
typedef char ActiveBody_off_m_lastDamageTimestamp[(offsetof(ActiveBody, m_lastDamageTimestamp) == 0xBC) ? 1 : -1];
typedef char ActiveBody_off_m_particleSystems[(offsetof(ActiveBody, m_particleSystems) == 0xC8) ? 1 : -1];
typedef char ActiveBody_off_m_damageEffects[(offsetof(ActiveBody, m_damageEffects) == 0xE0) ? 1 : -1];
typedef char ActiveBody_off_m_armorSetFlags[(offsetof(ActiveBody, m_armorSetFlags) == 0xF0) ? 1 : -1];
typedef char ActiveBody_off_m_curArmor[(offsetof(ActiveBody, m_curArmor) == 0xF8) ? 1 : -1];

#endif
