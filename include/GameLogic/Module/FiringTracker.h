// FiringTracker.h: the per-object helper that tracks consecutive shots at a victim (continuous-fire bonus model
// conditions), the force-reload timer and the looping-sound timer. Promoted from the Draft (Draft/FiringTracker.h).
// Target facts (retail game.dat): the constructor 0x004DEB10 calls UpdateModule first, stores the tables
// 0x00C61530 at +0x0, 0x00BEFF90 at +0xC and 0x00C61524 at +0x10, then sets the members below; update 0x004DED3B,
// calcTimeToSleep 0x004DEAB0 and coolDown 0x004DEC88 are reached through the +0x10 subobject (this - 0x10).
// xfer 0x004DEBC1 (after UpdateModule::xfer): version 1..2, then the int at +0x20, the ObjectIDs at +0x24 and +0x38,
// the frames at +0x3C/+0x40/+0x44, the Coord3D at +0x48, the bool at +0x34, the Coord3D at +0x28 and, from version 2,
// the frame at +0x54; the audio handle +0x58 goes through TheAudio->xferAudioHandle. Names are Zero Hour's where its
// FiringTracker has the same member (consecutive shots, victim, the three frames, audio handle); the rest are offsets.
#pragma once
#ifndef BFME2_FIRINGTRACKER_H
#define BFME2_FIRINGTRACKER_H

#include "GameLogic/Module/UpdateModule.h"
#include "Common/Coord3D.h"
#include "Common/GameAudio.h"

class Weapon;
class Object;
class Xfer;

class FiringTracker : public UpdateModule
{
public:
	FiringTracker(Thing *thing, const ModuleData *moduleData);
	virtual ~FiringTracker();											// out of line: slot 0 0x004DEA0C
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();									// 0x004DED3B

	void shotFired(const Weapon *weaponFired, ObjectID victimID, const Coord3D *victimPos, Bool flag);	// 0x004DEECB
	Int takeUnk0x38();					// 0x004DEA68: returns +0x38 and clears it

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes
	virtual DisabledMaskType getDisabledTypesToProcess() const;	// update slot 1: retail 0x004DF396 (DISABLEDMASK_ALL), ICF-folded with SlowDeathBehavior's
	virtual SleepyUpdatePhase getUpdatePhase() const { return (SleepyUpdatePhase)3; }	// slot 12: retail 0x0044827C (`push 3; pop eax`), UpdateModule's 0x0022C4CB answers 2

private:
	friend class WeaponFireSpecialAbilityUpdate;	// WeaponFireSpecialAbilityUpdate::triggerAbilityEffect 0x00492A72 calls coolDown 0x004DEC88
	void coolDown(Bool force);											// 0x004DEC88
	void speedUp();														// 0x004DEDD2
	UpdateSleepTime calcTimeToSleep();									// 0x004DEAB0: forever when none of the three timers is set

public:
	ObjectID getLastShotVictim() const { return m_victimID; }	// Zero Hour's (Object::getLastVictimID 0x0028AF86 reads +0x24 in place)

	Int m_consecutiveShots;			// +0x20
	ObjectID m_victimID;			// +0x24
	Coord3D m_unk0x28;				// +0x28, constructor zero
	Bool m_unk0x34;					// +0x34
	ObjectID m_unk0x38;				// +0x38
	UnsignedInt m_frameToStartCooldown;	// +0x3C
	UnsignedInt m_frameToForceReload;	// +0x40
	UnsignedInt m_unk0x44;			// +0x44
	Coord3D m_unk0x48;				// +0x48, the position update compares the object's against
	UnsignedInt m_frameToStopLoopingSound;	// +0x54
	AudioHandle m_audioHandle;		// +0x58
};
// SIZE 0x5C
typedef char FiringTracker_size[(sizeof(FiringTracker) == 0x5C) ? 1 : -1];
typedef char FiringTracker_off_m_unk0x28[(offsetof(FiringTracker, m_unk0x28) == 0x28) ? 1 : -1];
typedef char FiringTracker_off_m_frameToStartCooldown[(offsetof(FiringTracker, m_frameToStartCooldown) == 0x3C) ? 1 : -1];
typedef char FiringTracker_off_m_unk0x48[(offsetof(FiringTracker, m_unk0x48) == 0x48) ? 1 : -1];
typedef char FiringTracker_off_m_audioHandle[(offsetof(FiringTracker, m_audioHandle) == 0x58) ? 1 : -1];

#endif
