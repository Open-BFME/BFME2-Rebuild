// ReflectDamage.h: the ReflectDamage module. SIZE 0x14 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004BABB4 calls DamageModule first; it stores the
// tables 0x00C59CBC at +0x0, 0x00C597F8 at +0xC, 0x00C59CB0 at +0x10. Members are what xfer (0x0045CE8C) transfers after DamageModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004BAB4F ?name@Rva004BAB4FNamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004BABFA ?onDamage@ReflectDamage@@UAEXPAVDamageInfo@@@Z
//   +0x10 slot 1 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 2 -> 0x000D1407 ?Set_Animation@RenderObjClass@@UAEXPAVHAnimClass@@MH@Z
//   +0x10 slot 8 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 12 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 13 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
#pragma once
#ifndef BFME2_REFLECT_DAMAGE_H
#define BFME2_REFLECT_DAMAGE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DamageModule.h"

class Thing;
class ModuleData;
class Xfer;

// DamageModule ends at 0x14; this class's members follow it.
typedef char ReflectDamage_base_size[(sizeof(DamageModule) == 0x14) ? 1 : -1];

class DamageInfo;
class ReflectDamage : public DamageModule
{
public:
	ReflectDamage(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~ReflectDamage();
	virtual void onDamage(DamageInfo *damageInfo);	// override: onDamage slot 0 at +0x10 (0x004BABFA)

protected:
	virtual void xfer(Xfer *xfer);

public:
public:
	virtual void onHealing(DamageInfo *damageInfo) {}	// DamageModuleInterface slot 1: retail's is the shared `ret 4` 0x0047A69C; retail's table has this class's own (h05, gen3 vtable-content list)
	virtual void onBodyDamageStateChange(const DamageInfo *, BodyDamageType, BodyDamageType) {}
protected:
	virtual void loadPostProcess() { DamageModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to DamageModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char ReflectDamage_size[(sizeof(ReflectDamage) == 0x14) ? 1 : -1];

#endif
