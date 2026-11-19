// PropagandaTowerBehavior.h: the propaganda tower's update-and-die module (heals and cheers up the
// friendly objects around it). SIZE 0x30 (ModuleFactory operator new).
// Layout is Zero Hour's: class PropagandaTowerBehavior : public UpdateModule, public DieModuleInterface.
// Target facts (retail game.dat):
// - Ctor 0x004819CF calls UpdateModule's, then installs, at +0 the primary table 0x00C49248, at +0x0C the
//   behavior interface 0x00C49188, at +0x10 the update interface 0x00C4917C and at +0x20 the die interface
//   0x00C49178 (one slot, onDie). The only extra base is DieModuleInterface at +0x20. It zeroes +0x24, +0x28
//   and +0x2C and calls setWakeFrame(m_object, UPDATE_SLEEP_NONE).
// - Dtor 0x0048180C restores the same four tables and jumps to ~UpdateModule 0x0024A797; its deleting
//   thunk is 0x00481A38 (primary slot 0).
// - Primary table 0x00C49248 (16 slots; 0x00C49288 after it is the module data's table):
//   1 loadPostProcess 0x00497805 (jmp to UpdateModule's, ICF-folded across classes), 2 GetSnapshotName
//   0x0048182C, 3 xfer 0x00481842, 4 getModuleNameKey 0x0048191A, 5 onObjectCreated 0x00481A54 (resolves
//   the upgrade name at module data +0x18 with TheUpgradeCenter->findUpgrade into +0x2C),
//   8 onDelete 0x00481832 (a jump through slot 13), 9 onCapture 0x00481A6E, 13 removeAllInfluence 0x00481D4C,
//   14 doScan 0x00481DA4, 15 effectLogic 0x00481A9E. Slot 12 getUpdatePhase is UpdateModule's.
// - Behavior interface slot 7 (getDie) 0x004543A9, the null-checked die interface pointer.
// - Update interface slot 0 update 0x00481C0C (this = +0x10), slot 1 getDisabledTypesToProcess 0x004DF396
//   (returns DISABLEDMASK_ALL), slot 2 UpdateModule's.
// - Die interface slot 0 onDie 0x00481837: this = +0x20; subtracts 0x20 and calls slot 13.
// - Members: +0x24 the last scan frame (xfer 0x00481842 transfers it as an unsigned int), +0x28 the head of
//   the list of tracked objects (a singly linked list of ObjectTracker nodes: vptr, ObjectID at +4, next at
//   +8; xfer writes its length as an unsigned short then each ObjectID), +0x2C the upgrade template
//   onObjectCreated resolved (effectLogic and doScan test it with Player::rva002AB87D and Object::hasUpgrade).
// Names follow Zero Hour's PropagandaTowerBehavior.h (EA, GPL-3.0-or-later): member names, the three
// protected virtuals and their order are the same in retail's tables.
#pragma once
#ifndef BFME2_PROPAGANDA_TOWER_BEHAVIOR_H
#define BFME2_PROPAGANDA_TOWER_BEHAVIOR_H

#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DieModuleInterface.h"

class Thing;
class ModuleData;
class Xfer;
class Player;
class Object;
class UpgradeTemplate;
class DamageInfo;
class PropagandaTowerBehaviorModuleData;

// One tracked object of the tower's area of influence (defined where it is used; Zero Hour keeps it
// in PropagandaTowerBehavior.cpp too).
class ObjectTracker;

// UpdateModule ends at 0x20; the die interface and this class's members follow it.
typedef char PropagandaTowerBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class PropagandaTowerBehavior : public UpdateModule, public DieModuleInterface
{
public:
	PropagandaTowerBehavior(Thing *thing, const ModuleData *moduleData);	// 0x004819CF
	virtual ~PropagandaTowerBehavior();									// 0x0048180C (deleting thunk 0x00481A38)
	virtual NameKeyType getModuleNameKey() const;							// 0x0048191A
	virtual void onObjectCreated();										// slot 5, 0x00481A54
	virtual void onDelete();												// slot 8, 0x00481832
	virtual void onCapture(Player *oldOwner, Player *newOwner);			// slot 9, 0x00481A6E
	virtual UpdateSleepTime update();										// override: update slot 0 at +0x10 (0x00481C0C)
	virtual DisabledMaskType getDisabledTypesToProcess() const;			// override: update slot 1 at +0x10 (0x004DF396)
	virtual DieModuleInterface *getDie() { return this; }					// override: BehaviorModuleInterface slot 7 (retail 0x004543A9)
	virtual void onDie(const DamageInfo *damageInfo);						// override: die slot 0 at +0x20 (0x00481837)

	virtual const char *GetSnapshotName() const;							// slot 2, 0x0048182C

protected:
	virtual void xfer(Xfer *xfer);											// slot 3, 0x00481842
	virtual void removeAllInfluence();										// slot 13, 0x00481D4C
	virtual void doScan();													// slot 14, 0x00481DA4
	virtual void effectLogic(Object *obj, Bool giving, const PropagandaTowerBehaviorModuleData *moduleData);	// slot 15, 0x00481A9E
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes

	UnsignedInt m_lastScanFrame;											// +0x24 xfer 0x00481842 (unsigned int); update 0x00481C0C
	ObjectTracker *m_insideList;											// +0x28 xfer's count loop; removeAllInfluence 0x00481D4C
	const UpgradeTemplate *m_upgradeRequired;								// +0x2C onObjectCreated 0x00481A54; effectLogic 0x00481A9E
};
// SIZE 0x30
typedef char PropagandaTowerBehavior_size[(sizeof(PropagandaTowerBehavior) == 0x30) ? 1 : -1];

#endif
