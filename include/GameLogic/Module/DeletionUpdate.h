// DeletionUpdate.h: counts a lifetime down and destroys its object when it runs out.
// SIZE 0x24 (ModuleFactory operator new).
// Target facts (retail image, h05 modgen and the DeletionUpdate rows): the constructor
// 0x004883D0 calls UpdateModule first, then stores the primary table 0x00C4B338 at +0x00,
// 0x00BEFF90 at +0x0C and 0x00C4B32C at +0x10. The module data's minimum and maximum frame
// counts are read by the constructor and by setLifetimeRange; calcSleepDelay stores the
// frame the object dies on at +0x20. xfer (0x00488483) transfers UpdateModule first, then
// that one word, typed by the Xfer slot it goes through (operator==(unsigned int &)).
// update (0x0048846F) destroys the object and sleeps forever; it is the slot 0 entry of the
// UpdateModuleInterface table at +0x10.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
#pragma once
#ifndef BFME2_DELETION_UPDATE_H
#define BFME2_DELETION_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char DeletionUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class DeletionUpdate : public UpdateModule
{
public:
	DeletionUpdate(Thing *thing, const ModuleData *moduleData);

	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();

	void setLifetimeRange(UnsignedInt minFrames, UnsignedInt maxFrames);

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	UnsignedInt calcSleepDelay(UnsignedInt minFrames, UnsignedInt maxFrames);

public:
	UnsignedInt m_dieFrame;		// +0x20; the frame the object is destroyed on
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x24
typedef char DeletionUpdate_size[(sizeof(DeletionUpdate) == 0x24) ? 1 : -1];
typedef char DeletionUpdate_off_m_dieFrame[(offsetof(DeletionUpdate, m_dieFrame) == 0x20) ? 1 : -1];

#endif
