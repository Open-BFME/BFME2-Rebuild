// DRAFT (h05 modgen): generated from constructor RVA 0x004C18CA and xfer RVA 0x004C1974; no function
// matches against it yet, so it stays in Draft until one does.
// FreeLifeBody.h: the FreeLifeBody module. SIZE 0x114 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C18CA calls RespawnBody first; it stores the
// tables 0x00C5BE38 at +0x0, 0x00C5AD78 at +0xC, 0x00C5BD88 at +0x10. Members are what xfer (0x004C1974) transfers after RespawnBody::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 2 -> 0x004C187F ?name@Rva004C187FNamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004C193C ?attemptDamage@Rva004C193COwner@@QAEXPAVDamageInfo@@@Z
//   +0x10 slot 32 -> 0x004C19DC ?apply@Rva004C19DCOwner@@QAEXMPAVDamageInfo@@@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@PorcupineFormationBodyModule@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_FREE_LIFE_BODY_H
#define BFME2_FREE_LIFE_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/RespawnBody.h"

class Thing;
class ModuleData;
class Xfer;

// RespawnBody ends at 0x100; this class's members follow it.
typedef char FreeLifeBody_base_size[(sizeof(RespawnBody) == 0x100) ? 1 : -1];

class FreeLifeBody : public RespawnBody
{
public:
	FreeLifeBody(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x100;                               // +0x100; xfer Bool
	Real m_unk0x104;                               // +0x104; xfer Real
	Bool m_unk0x108;                               // +0x108; xfer Bool
	UnsignedInt m_unk0x10C;                        // +0x10C; xfer UnsignedInt
	UnsignedInt m_unk0x110;                        // +0x110; xfer UnsignedInt
};
// SIZE 0x114
typedef char FreeLifeBody_size[(sizeof(FreeLifeBody) == 0x114) ? 1 : -1];
typedef char FreeLifeBody_off_m_unk0x100[(offsetof(FreeLifeBody, m_unk0x100) == 0x100) ? 1 : -1];
typedef char FreeLifeBody_off_m_unk0x104[(offsetof(FreeLifeBody, m_unk0x104) == 0x104) ? 1 : -1];
typedef char FreeLifeBody_off_m_unk0x108[(offsetof(FreeLifeBody, m_unk0x108) == 0x108) ? 1 : -1];
typedef char FreeLifeBody_off_m_unk0x10C[(offsetof(FreeLifeBody, m_unk0x10C) == 0x10C) ? 1 : -1];
typedef char FreeLifeBody_off_m_unk0x110[(offsetof(FreeLifeBody, m_unk0x110) == 0x110) ? 1 : -1];

#endif
