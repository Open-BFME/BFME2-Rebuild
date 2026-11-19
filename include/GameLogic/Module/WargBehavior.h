// WargBehavior.h: the WargBehavior module. SIZE 0x10 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00460804 calls BehaviorModule first; it stores the
// tables 0x00C426E8 at +0x0, 0x00BEE9C0 at +0xC. Members are what xfer (0x004CE56D) transfers after BehaviorModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0058B03E ?loadPostProcess@UpdateModule@@MAEXXZ
//   +0x0 slot 2 -> 0x00460827 ?Rva00460827Get@@YAHXZ
#pragma once
#ifndef BFME2_WARG_BEHAVIOR_H
#define BFME2_WARG_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/BehaviorModule.h"

class Thing;
class ModuleData;
class Xfer;

// BehaviorModule ends at 0x10; this class's members follow it.
typedef char WargBehavior_base_size[(sizeof(BehaviorModule) == 0x10) ? 1 : -1];

class WargBehavior : public BehaviorModule
{
public:
	WargBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x10
typedef char WargBehavior_size[(sizeof(WargBehavior) == 0x10) ? 1 : -1];

#endif
