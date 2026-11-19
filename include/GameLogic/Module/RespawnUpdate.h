// RespawnUpdate.h: the respawn update module (a dead object's respawn timer). SIZE 0x44.
// Target facts (retail game.dat):
// - Layout: UpdateModule (0x20; its UpdateModuleInterface at +0x10). The members follow it.
// - Ctor 0x004AF096 calls the UpdateModule ctor, stores -1 into +0x28, +0x34, +0x38 and +0x3C,
//   0 into +0x24, +0x2C, +0x30 and the bytes at +0x40 and +0x41, -1.0f (ds:0xBBB9AC) into +0x20,
//   then calls setWakeFrame(getObject(), UPDATE_SLEEP_FOREVER).
// - xfer 0x004AF00B: the version pair {1,1}, then UpdateModule::xfer, then XferRespawnModeType
//   at +0x2C, XferObjectID at +0x30, int at +0x34, +0x38, +0x3C, real at +0x20 and bools at
//   +0x40 and +0x41 (the int/real/bool slots are Xfer's operator== slots 31, 28 and 36).
// - getPercentReady 0x004AEFAD reads +0x2C (the mode) and +0x34..+0x3C; its constants are
//   0.0f (ds:0xBBAEAC) and 1.0f (ds:0xBBB8D8).
// Not established: the member names (role names are placeholders below), the mode enum, and the
// frame-based branch of getPercentReady.
#pragma once
#ifndef BFME2_RESPAWNUPDATE_H
#define BFME2_RESPAWNUPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;
class Object;
class Image;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char RespawnUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class RespawnUpdate : public UpdateModule
{
public:
	virtual DisabledMaskType getDisabledTypesToProcess() const;	// override: update slot 1 at +0x10 (retail 0x004DF396, DISABLEDMASK_ALL) (f08)
	RespawnUpdate(Thing *thing, const ModuleData *moduleData);	// 0x004AF096
	virtual NameKeyType getModuleNameKey() const;				// 0x004AF116
	virtual Real getPercentReady(Int *remaining);				// 0x004AEFAD (primary slot 13)
	virtual const Image *getDeadImage();						// 0x004AF2DE (primary slot 14)
	virtual Bool slot15() const;								// primary slot 15: 0x005C4FEB returns the +0x41 flag (a 4-byte getter the map files as ?get@RespawnUpdate@@QBEEXZ); out of line until the map accepts this name
	void rva004AEF46();											// 0x004AEF46 (starts the respawn from idle)
	void rva004AF182();											// 0x004AF182 (leaves an active respawn)
	void *rva004AF25D();										// 0x004AF25D (lazy respawn-as template)
	Object *searchForSuitableSpawnAtObject();					// 0x004AF1CE
	void triggerDeathBeforeRespawn();							// 0x004AF63E (public in the map: QAE)
	void triggerRevival(Object *at);							// 0x004AF79C (public in the map: QAE)
	Int getCostToRespawn();										// 0x004AFB01: module data's calcCostToBuild at the object's experience level (+0x264 -> +0x24)
	Int getTimeToRespawn();										// 0x004AFB16: module data's calcTimeToBuild at the same level

protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: the UpdateModule forward, ICF-folded across classes (primary slot 1)
	virtual const char *GetSnapshotName() const;				// slot 2: 0x004AF15B
	virtual void xfer(Xfer *xfer);								// 0x004AF00B

public:
	Real m_unk20;												// +0x20
	const Image *m_deadImage;									// +0x24 the cached image getDeadImage looks up (0 until then)
	void *m_unk28;												// +0x28 template cache for rva004AF25D; -1 until first use
	Int m_respawnMode;											// +0x2C (XferRespawnModeType)
	ObjectID m_unk30;											// +0x30 (XferObjectID)
	Int m_unk34;												// +0x34
	Int m_unk38;												// +0x38
	Int m_unk3C;												// +0x3C
	Bool m_unk40;												// +0x40
	Bool m_unk41;												// +0x41
public:
	virtual UpdateSleepTime update();	// 0x004AF92F, UpdateModuleInterface slot 0; retail's table has this class's own (h05, gen3 vtable-content list)
};
// SIZE 0x44
typedef char RespawnUpdate_size[(sizeof(RespawnUpdate) == 0x44) ? 1 : -1];
typedef char RespawnUpdate_off_m_unk20[(offsetof(RespawnUpdate, m_unk20) == 0x20) ? 1 : -1];
typedef char RespawnUpdate_off_m_respawnMode[(offsetof(RespawnUpdate, m_respawnMode) == 0x2C) ? 1 : -1];
typedef char RespawnUpdate_off_m_unk41[(offsetof(RespawnUpdate, m_unk41) == 0x41) ? 1 : -1];

#endif
