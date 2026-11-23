// The height-death update waits for its configured initial delay and then
// checks the object's position against the death and particle thresholds.
// Update 0x0048FD1B uses the +0x10 UpdateModuleInterface receiver. It computes
// the first check frame at 0x0048FD44, sets the death latch after Object::kill
// at 0x0048FF27, and sets the particle latch after its threshold callback
// at 0x0048FF56. The ending three stores preserve position for the next check.
#pragma once
#ifndef BFME2_HEIGHTDIEUPDATE_H
#define BFME2_HEIGHTDIEUPDATE_H

#include "Common/Coord3D.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/HeightDieUpdateModuleData.h"

class Xfer;

class HeightDieUpdate : public UpdateModule
{
public:
	HeightDieUpdate(Thing *thing, const ModuleData *moduleData);
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	virtual ~HeightDieUpdate();

	virtual void loadPostProcess(void);
	virtual NameKeyType getModuleNameKey(void) const;
	const HeightDieUpdateModuleData *getHeightDieUpdateModuleData() const { return static_cast<const HeightDieUpdateModuleData *>(m_moduleData); }

protected:
	virtual const char *GetSnapshotName(void) const;
	virtual void xfer(Xfer *xfer);

public:
	Bool m_triggeredHeightDeath;			// +0x20
	Bool m_destroyedAttachedParticles;			// +0x21
	Coord3DBase m_lastPosition;		// +0x24
	UnsignedInt m_firstCheckFrame;		// +0x30
public:
	virtual UpdateSleepTime update();	// UpdateModuleInterface slot 0; retail's table has this class's own
};

#endif
