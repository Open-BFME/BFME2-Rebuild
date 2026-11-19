// DamageModule.h: the interface a module exposes when it takes damage.
// Target facts (old repo reference/shims/bfme2_flammableupdate/GameLogic/Module/
// DamageModule.h): three slots and no virtual destructor -- onDamage,
// onHealing, onBodyDamageStateChange. BehaviorModuleInterface::getDamage is slot
// 4 of the behavior interface and returns this base. FlammableUpdate's table
// sits at +0x20 (the dtor restores it after the UpdateModuleInterface one).
#pragma once
#ifndef BFME2_DAMAGEMODULE_H
#define BFME2_DAMAGEMODULE_H

#include "GameLogic/Damage.h"
#include "GameLogic/Module/BehaviorModule.h"
#include "Common/Xfer.h"

class DamageInfo;
class Xfer;

class DamageModuleInterface
{
public:
	virtual void onDamage(DamageInfo *damageInfo) = 0;						// 0
	virtual void onHealing(DamageInfo *damageInfo) = 0;						// 1
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo,
		BodyDamageType oldState, BodyDamageType newState) = 0;				// 2
};

// DamageModule: the module base a damage-taking behavior derives.
// Target facts: ctor 0x004B9704 and GetSnapshotName 0x004B9735; BoneFXDamage's ctor
// 0x004B979C calls the ctor and then stores its own tables at +0 and +0x10, so
// DamageModule is BehaviorModule (primary +0, behavior interface +0x0C) plus the
// DamageModuleInterface vptr at +0x10 and adds no member.
class DamageModule : public BehaviorModule, public DamageModuleInterface
{
public:
	DamageModule(Thing *thing, const ModuleData *moduleData);

	// BehaviorModuleInterface slot 4 returns this module's damage interface at +0x10.
	virtual DamageModuleInterface *getDamage() { return this; }

protected:
	// Out of line: retail's is the fold 0x004B96CC (stores 0x00BE2B78 at +0x10, BehaviorModule's
	// tables, then jumps to ~ObjectModule); BoneFXDamage 0x004B96E5 and ReflectDamage 0x004BAB36
	// store their own three tables and tail-jump to it. The map names it only by placeholder.
	virtual ~DamageModule();
	virtual const char *GetSnapshotName() const;
	// The fold 0x004CE56D (Collide/Die/Damage modules): Version 1, then BehaviorModule's.
	virtual void xfer(Xfer *xfer)
	{
		xfer->Version1();
		BehaviorModule::xfer(xfer);
	}
protected:
	virtual void loadPostProcess();	// out of line (a jmp to BehaviorModule's), ICF-folded with UpdateModule's at 0x0058B03E; inline, MSVC folds it into each subclass's forwarder and loses retail's second level (0x00497805) (h05)
};
// SIZE 0x14 (h05): BehaviorModule (0x10) plus the DamageModuleInterface vptr at +0x10 and no
// member, as the target facts above say; BoneFXDamage's constructor stores at +0 and +0x10 only.
typedef char DamageModule_size[(sizeof(DamageModule) == 0x14) ? 1 : -1];

#endif
