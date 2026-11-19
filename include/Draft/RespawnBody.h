// DRAFT (h05 modgen): generated from constructor RVA 0x004C12EB and xfer RVA 0x004C20BA; no function
// matches against it yet, so it stays in Draft until one does.
// RespawnBody.h: the RespawnBody module. SIZE 0x100 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C12EB calls ActiveBody first; it stores the
// tables 0x00C5B9A0 at +0x0, 0x00C5AD78 at +0xC, 0x00C5B8F0 at +0x10. Members are what xfer (0x004C20BA) transfers after ActiveBody::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004C202F ?rva004C202F@Rva004C202F@@QAEXXZ
//   +0x0 slot 2 -> 0x004C1315 ?Rva004C1315Get@@YAHXZ
//   +0x10 slot 32 -> 0x004C1395 ?internalChangeHealth@RespawnBody@@UAEXMPAVDamageInfo@@@Z
#pragma once
#ifndef BFME2_RESPAWN_BODY_H
#define BFME2_RESPAWN_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBody.h"

class Thing;
class ModuleData;
class Xfer;

// ActiveBody ends at 0x100; this class's members follow it.
typedef char RespawnBody_base_size[(sizeof(ActiveBody) == 0x100) ? 1 : -1];

class RespawnBody : public ActiveBody
{
public:
	RespawnBody(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void internalChangeHealth(Real, DamageInfo *);	// override: internalChangeHealth slot 32 at +0x10 (0x004C1395)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x100
typedef char RespawnBody_size[(sizeof(RespawnBody) == 0x100) ? 1 : -1];

#endif
