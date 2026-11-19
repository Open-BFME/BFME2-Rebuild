// DRAFT (h05 modgen): generated from constructor RVA 0x004C0AF1 and xfer RVA 0x004C0A76; no function
// matches against it yet, so it stays in Draft until one does.
// SymbioticStructuresBody.h: the SymbioticStructuresBody module. SIZE 0x10C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C0AF1 calls ActiveBody first; it stores the
// tables 0x00C5B830 at +0x0, 0x00C5B770 at +0xC, 0x00C5B6C0 at +0x10. Members are what xfer (0x004C0A76) transfers after ActiveBody::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004C1170 ?loadPostProcess@SymbioticStructuresBody@@MAEXXZ
//   +0x0 slot 2 -> 0x004C0A65 ?name@Rva004C0A65Named@@QBEPBDXZ
//   +0x0 slot 21 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 23 -> 0x004C0B60 ?rva004C0B60@SymbioticStructuresBody@@UAEXURva004C0B60Arg@@@Z
//   +0x0 slot 24 -> 0x004C0D9C ?rva004C0D9C@SymbioticStructuresBody@@UAEMXZ
//   +0xC slot 45 -> 0x004C0A6B ?rva004C0A6B@Rva004C0A6BInterface@@QAEXH@Z
//   +0x10 slot 1 -> 0x004C0DD1 ?bfmeGoBUE@BfmeThingBUE@@QAEXPAX@Z
//   +0x10 slot 2 -> 0x004C1028 ?rva004C1028@Rva004C1028@@QAEMH@Z
//   +0x10 slot 3 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 5 -> 0x004C0D13 ?rva004C0D13@SymbioticStructuresBody@@UAEMXZ
//   +0x10 slot 6 -> 0x004C105A ?rva004C105A@SymbioticStructuresBody@@UAEMXZ
//   +0x10 slot 7 -> 0x004C108B ?rva004C108B@SymbioticStructuresBody@@UAEMXZ
//   +0x10 slot 8 -> 0x004C0E68 ?bfmeGoCAE@BfmeThingCAE@@QAEHXZ
//   +0x10 slot 11 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 14 -> 0x004C0E8B ?rva004C0E8B@Rva004C0E8B@@QAEPAXPAX@Z
//   +0x10 slot 15 -> 0x004C0E46 ?bfmeGoDHB@BfmeThingDHB@@QAEHXZ
//   +0x10 slot 16 -> 0x004C0F6F ?bfmeGoBVF@BfmeThingBVF@@QAEHXZ
//   +0x10 slot 17 -> 0x004C0F92 ?bfmeGoBVG@BfmeThingBVG@@QAEHXZ
//   +0x10 slot 18 -> 0x004C1006 ?bfmeGoDHC@BfmeThingDHC@@QAEHXZ
//   +0x10 slot 19 -> 0x004C1121 ?bfmeGoCCD@BfmeThingCCD@@QAEHXZ
//   +0x10 slot 27 -> 0x004C0DF3 ?rva004C0DF3@SymbioticStructuresBody@@UAEMXZ
//   +0x10 slot 28 -> 0x004C0ECE ?rva004C0ECE@SymbioticStructuresBody@@UAE?AVAsciiString@@XZ
//   +0x10 slot 29 -> 0x004C0E24 ?bfmeGoDHA@BfmeThingDHA@@QAEHXZ
//   +0x10 slot 30 -> 0x004C10F0 ?rva004C10F0@Rva004C10F0@@QAEMXZ
//   +0x10 slot 31 -> 0x004C0FB5 ?rva004C0FB5@Rva004C0FB5@@QAEPAXPAX@Z
//   +0x10 slot 34 -> 0x004C1147 ?bfmeGoCFC@BfmeThingCFC@@QAEHXZ
//   +0x10 slot 35 -> 0x004C10BC ?rva004C10BC@Rva004C10BC@@QAEMXZ
//   +0x10 slot 38 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x10 slot 39 -> 0x004C121B ?rva004C121B@SymbioticStructuresBody@@UAEXPAX@Z
//   +0x10 slot 40 -> 0x004C123C ?rva004C123C@SymbioticStructuresBody@@UAEXPAX_N@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_SYMBIOTIC_STRUCTURES_BODY_H
#define BFME2_SYMBIOTIC_STRUCTURES_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBody.h"

class Thing;
class ModuleData;
class Xfer;

// ActiveBody ends at 0x100; this class's members follow it.
typedef char SymbioticStructuresBody_base_size[(sizeof(ActiveBody) == 0x100) ? 1 : -1];

class SymbioticStructuresBody : public ActiveBody
{
public:
	SymbioticStructuresBody(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();	// override: loadPostProcess slot 1 at +0x0 (0x004C1170)

	// BodyModuleInterface overrides (+0x10): each connects to the host body and forwards to it, or answers zero.
	virtual void attemptHealing(DamageInfo *damageInfo);					// 1, 0x004C0DD1
	virtual BodyDamageType getDamageState() const;						// 8, 0x004C0E68 (BODY_RUBBLE without a host)
	virtual const DamageInfo *getLastDamageInfo() const;				// 15, 0x004C0E46
	virtual unsigned int getLastDamageTimestamp() const;				// 16, 0x004C0F6F (-1 without a host)
	virtual unsigned int getLastHealingTimestamp() const;				// 17, 0x004C0F92 (-1 without a host)
	virtual ObjectID getAndClearLastAttacker();										// 18, 0x004C1006
	virtual unsigned char getFrontCrushed() const;							// 19, 0x004C1121 (retail's slot 20 holds the same body)
	virtual int getGrabFX() const;									// 29, 0x004C0E24
	virtual unsigned char isIndestructible() const;							// 34, 0x004C1147
	virtual Real estimateDamage(DamageInfoInput &input) const;			// 2, 0x004C1028
	virtual Real getHealthRatio() const;								// 5, 0x004C0D13 (refreshes the host indicators first)
	virtual Real getMaxHealth() const;									// 6, 0x004C105A
	virtual Real getInitialHealth() const;									// 7, 0x004C108B
	virtual Real getDamageScalar() const;								// 27, 0x004C0DF3
	virtual AsciiString getGrabObjectName() const;							// 28, 0x004C0ECE (empty without a host)
	virtual Real getGrabDamage() const;									// 30, 0x004C10F0
	virtual Real getDodgePercent() const;									// 35, 0x004C10BC
	virtual void rvaSlot39(BodyDamageType state);						// 39, 0x004C121B (rva0028FCD9 on the owner, then refreshHostDrawables)

	Bool connectToHostBody() const;										// 0x004C0D4F (map: Rva004C0D4F): finds the host object's body from m_hostId and tells it this object's id
	void refreshHostDrawables() const;									// 0x004C0C52: flashes the drawables of the host and of this object (mode 5 on the one the local player does not own)

public:
	mutable ActiveBody *m_hostBody;                // +0x100; connectToHostBody stores the host object's body module here
	ObjectID m_hostId;                             // +0x104; xfer ObjectID: the host object
	UnsignedByte m_gap0x108[0x4];                  // +0x108; slot 14 (0x004C0E8B) answers with a BitFlags<21> here when there is no host (not written yet)
};
// SIZE 0x10C
typedef char SymbioticStructuresBody_size[(sizeof(SymbioticStructuresBody) == 0x10C) ? 1 : -1];
typedef char SymbioticStructuresBody_off_m_hostId[(offsetof(SymbioticStructuresBody, m_hostId) == 0x104) ? 1 : -1];

#endif
