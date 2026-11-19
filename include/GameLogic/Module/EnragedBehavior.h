// EnragedBehavior.h: the EnragedBehavior module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00458FA3 calls UpdateModule first; it stores the
// tables 0x00C40FBC at +0x0, 0x00BEFF90 at +0xC, 0x00C40FB0 at +0x10. Members are what xfer (0x00459047) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00458FFC ?Rva458FFCGet@@YAHXZ
//   +0x10 slot 0 -> 0x00459109 ?rva00459109@Rva00459109@@QAEHXZ
#pragma once
#ifndef BFME2_ENRAGED_BEHAVIOR_H
#define BFME2_ENRAGED_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char EnragedBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class EnragedBehavior : public UpdateModule
{
public:
	EnragedBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Real m_unk0x20;                                // +0x20; xfer Real
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x24
typedef char EnragedBehavior_size[(sizeof(EnragedBehavior) == 0x24) ? 1 : -1];
typedef char EnragedBehavior_off_m_unk0x20[(offsetof(EnragedBehavior, m_unk0x20) == 0x20) ? 1 : -1];

#endif
