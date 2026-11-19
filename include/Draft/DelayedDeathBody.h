// DRAFT (h05 modgen): generated from constructor RVA 0x004C15B9 and xfer RVA 0x004C17CC; no function
// matches against it yet, so it stays in Draft until one does.
// DelayedDeathBody.h: the DelayedDeathBody module. SIZE 0x104 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C15B9 calls RespawnBody first; it stores the
// tables 0x00C5BBE8 at +0x0, 0x00C5AD78 at +0xC, 0x00C5BB38 at +0x10. Members are what xfer (0x004C17CC) transfers after RespawnBody::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 2 -> 0x004C15F1 ?Rva004C15F1Get@@YAHXZ
//   +0x10 slot 32 -> 0x004C167E ?internalChangeHealth@DelayedDeathBody@@QAEXMPAVDamageInfo@@@Z
//   +0x10 slot 37 -> 0x004C15F7 ?set@Rva004C15F7ByteSlot@@QAEXE@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@PorcupineFormationBodyModule@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_DELAYED_DEATH_BODY_H
#define BFME2_DELAYED_DEATH_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/RespawnBody.h"

class Thing;
class ModuleData;
class Xfer;

// RespawnBody ends at 0x100; this class's members follow it.
typedef char DelayedDeathBody_base_size[(sizeof(RespawnBody) == 0x100) ? 1 : -1];

class DelayedDeathBody : public RespawnBody
{
public:
	DelayedDeathBody(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void internalChangeHealth(Real, DamageInfo *);	// override: internalChangeHealth slot 32 at +0x10 (0x004C167E)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x100;                               // +0x100; xfer Bool
	Bool m_unk0x101;                               // +0x101; xfer Bool
};
// SIZE 0x104
typedef char DelayedDeathBody_size[(sizeof(DelayedDeathBody) == 0x104) ? 1 : -1];
typedef char DelayedDeathBody_off_m_unk0x100[(offsetof(DelayedDeathBody, m_unk0x100) == 0x100) ? 1 : -1];
typedef char DelayedDeathBody_off_m_unk0x101[(offsetof(DelayedDeathBody, m_unk0x101) == 0x101) ? 1 : -1];

#endif
