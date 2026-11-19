// BoredUpdate.h: the BoredUpdate module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (retail game.dat): the constructor 0x00496791 calls UpdateModule first, then
// stores this class's tables (its own primary, the UpdateModule interface and the behavior
// interface). Members are what xfer (0x004ACAAA) transfers after UpdateModule::xfer; the one
// member not yet named is at +0x20 (UnsignedInt). Bytes xfer does not touch are gaps.
#pragma once
#ifndef BFME2_BORED_UPDATE_H
#define BFME2_BORED_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char BoredUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class BoredUpdate : public UpdateModule
{
public:
	BoredUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~BoredUpdate();
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
public:
	virtual UpdateSleepTime update();	// 0x0049680F (UpdateModuleInterface slot 0)
};
// SIZE 0x24
typedef char BoredUpdate_size[(sizeof(BoredUpdate) == 0x24) ? 1 : -1];
typedef char BoredUpdate_off_m_unk0x20[(offsetof(BoredUpdate, m_unk0x20) == 0x20) ? 1 : -1];

#endif
