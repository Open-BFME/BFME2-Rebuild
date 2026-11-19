// InactiveBody.h: the InactiveBody module. SIZE 0x1C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004BD8BD calls BodyModule first; it stores the
// tables 0x00C5AE34 at +0x0, 0x00C5AD78 at +0xC, 0x00C5ACC8 at +0x10. Members are what xfer (0x004BD9A1) transfers after BodyModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004BD7A8 ?name@Rva004BD7A8Named@@QBEPBDXZ
//   +0x0 slot 14 -> 0x0058B03E ?loadPostProcess@UpdateModule@@MAEXXZ
//   +0x0 slot 15 -> 0x004BD83B ?Rva004BD83BGet@@YAHXZ
//   +0x0 slot 17 -> 0x0003B810 __purecall
//   +0x0 slot 18 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 22 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 23 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 25 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 0 -> 0x004BD92C ?attemptDamage@InactiveBody@@UAEXPAVDamageInfo@@@Z
//   +0x10 slot 1 -> 0x004BD7D4 ?attemptHealing@InactiveBody@@UAEXPAVDamageInfo@@@Z
//   +0x10 slot 2 -> 0x004BD7AE ?Rva004BD7AEGet@@YGMPAU_Rva004BD7AE@@@Z
//   +0x10 slot 4 -> 0x002BFBF0 ?vslot50@DrawModule@@UAEMXZ
//   +0x10 slot 9 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 14 -> 0x004BD97A ?rva004BD97A@@YGHH@Z
//   +0x10 slot 28 -> 0x004BD9CB ?rva004BD9CB@Rva004BD9CB@@UBE?AVAsciiString@@XZ
//   +0x10 slot 31 -> 0x004BD8A2 ?rva004BD8A2@Rva004BD8A2@@UAE?AUBodyZeroPair004BD8A2@@XZ
#pragma once
#ifndef BFME2_INACTIVE_BODY_H
#define BFME2_INACTIVE_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/BitFlags.h"
#include "Common/AsciiString.h"
#include "GameLogic/Module/BodyModule.h"

class Thing;
class ModuleData;
class Xfer;

// BodyModule ends at 0x18; this class's members follow it.
typedef char InactiveBody_base_size[(sizeof(BodyModule) == 0x18) ? 1 : -1];

class DamageInfoInput;
class InactiveBody : public BodyModule
{
public:
	InactiveBody(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual void attemptDamage(DamageInfo *damageInfo);				// 0x004BD92C
	virtual void attemptHealing(DamageInfo *damageInfo);				// 0x004BD7D4
	// The rest of BodyModuleInterface (retail's table 0x00C5ACC8): the slots BodyModule leaves pure are the shared stubs
	// 0x000B3FD0 (ret), 0x000D43D0 (return 0), 0x002BFBF0 (return 0.0f), 0x0047A69C (ret 4) and 0x0050B238 (ret 8).
	virtual Real getHealth() const;										// 4: 0.0f
	virtual Real getHealthRatio() const;								// 5: 0.0f
	virtual BodyDamageType getDamageState() const;						// 8: 0 (BODY_PRISTINE)
	virtual void setDamageState(BodyDamageType state);					// 9: ret 4
	virtual void updateBodyParticleSystems();							// 10: ret
	virtual void onVeterancyLevelChanged(VeterancyLevel oldLevel, VeterancyLevel newLevel);	// 11: ret 8
	virtual void setArmorSetFlag(ArmorSetType type);					// 12: ret 4
	virtual void clearArmorSetFlag(ArmorSetType type);					// 13: ret 4
	virtual BitFlags<21> getArmorSetFlags();									// 14: 0x004BD97A, an empty set (not written yet)
	virtual AsciiString getGrabObjectName() const;							// 28: 0x004BD9CB, the empty string (not written yet)
	virtual int getGrabFX() const;									// 29: 0
	virtual Real getGrabDamage() const;									// 30: 0.0f
	virtual Coord2D getGrabOffset();									// 31: 0x004BD8A2, two zero floats
	virtual void internalChangeHealth(Real amount, DamageInfo *damageInfo);	// 32: ret 8

protected:
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x18;                                // +0x18; xfer Bool
public:
	virtual Real estimateDamage(DamageInfoInput &damageInfo) const;	// 0x004BD7AE, BodyModuleInterface slot 2; retail's table has this class's own (h05, gen3 vtable-content list)
	virtual void reapplyDamageState() {}	// BodyModuleInterface slot 3: retail's table holds the shared empty body 0x000B3FD0, not pure (h05 r9)
protected:
	virtual void loadPostProcess() { BodyModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to BodyModule's, ICF-folded across classes (h05)
};
// SIZE 0x1C
typedef char InactiveBody_size[(sizeof(InactiveBody) == 0x1C) ? 1 : -1];
typedef char InactiveBody_off_m_unk0x18[(offsetof(InactiveBody, m_unk0x18) == 0x18) ? 1 : -1];

#endif
