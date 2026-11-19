// ExperienceLevelCreate.h: the ExperienceLevelCreate module. SIZE 0x18 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B917B calls CreateModule first; it stores the
// tables 0x00C5956C at +0x0, 0x00BEF9C0 at +0xC, 0x00C5955C at +0x10. Members are what xfer (0x004851E3) transfers after CreateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004B91A5 ?Rva004B91A5Get@@YAHXZ
//   +0x10 slot 0 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 1 -> 0x004B9231 ?onBuildComplete@ExperienceLevelCreate@@UAEXXZ
#pragma once
#ifndef BFME2_EXPERIENCE_LEVEL_CREATE_H
#define BFME2_EXPERIENCE_LEVEL_CREATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CreateModule.h"

class Thing;
class ModuleData;
class Xfer;

// CreateModule ends at 0x18; this class's members follow it.
typedef char ExperienceLevelCreate_base_size[(sizeof(CreateModule) == 0x18) ? 1 : -1];

class ExperienceLevelCreate : public CreateModule
{
public:
	ExperienceLevelCreate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void onBuildComplete();	// override: onBuildComplete slot 1 at +0x10 (0x004B9231)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { CreateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to CreateModule's (h05)
};
// SIZE 0x18
typedef char ExperienceLevelCreate_size[(sizeof(ExperienceLevelCreate) == 0x18) ? 1 : -1];

#endif
