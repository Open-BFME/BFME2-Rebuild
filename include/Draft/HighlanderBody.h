// DRAFT (h05 modgen): generated from constructor RVA 0x004C0781 and xfer RVA 0x004C20BA; no function
// matches against it yet, so it stays in Draft until one does.
// HighlanderBody.h: the HighlanderBody module. SIZE 0x100 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C0781 calls ActiveBody first; it stores the
// tables 0x00C5B440 at +0x0, 0x00C5AD78 at +0xC, 0x00C5B390 at +0x10. Members are what xfer (0x004C20BA) transfers after ActiveBody::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004C202F ?rva004C202F@Rva004C202F@@QAEXXZ
//   +0x0 slot 2 -> 0x004C07AB ?Rva004C07ABGet@@YAHXZ
//   +0x10 slot 0 -> 0x004C082B ?attemptDamage@HighlanderBody@@UAEXPAVDamageInfo@@@Z
#pragma once
#ifndef BFME2_HIGHLANDER_BODY_H
#define BFME2_HIGHLANDER_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBody.h"

class Thing;
class ModuleData;
class Xfer;
struct DamageInfo;

// ActiveBody ends at 0x100; this class's members follow it.
typedef char HighlanderBody_base_size[(sizeof(ActiveBody) == 0x100) ? 1 : -1];

class HighlanderBody : public ActiveBody
{
public:
	HighlanderBody(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~HighlanderBody();
	virtual void attemptDamage(DamageInfo *damageInfo);		// 0x004C082B (+0x10 slot 0)

protected:
	virtual void loadPostProcess() { ActiveBody::loadPostProcess(); }	// slot 1: retail 0x004C202F, a jmp to ActiveBody's (0x004BF40F), ICF-folded across the ActiveBody subclasses
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x100
typedef char HighlanderBody_size[(sizeof(HighlanderBody) == 0x100) ? 1 : -1];

#endif
