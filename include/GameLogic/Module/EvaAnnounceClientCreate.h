// EvaAnnounceClientCreate.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x004C9A0C);
// name, size, bases and virtual order are retail facts given below.
// EvaAnnounceClientCreate.h: the EvaAnnounceClientCreate module. SIZE 0x18 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C99D4 calls Rva00362EC7 first; it stores the
// tables 0x00C5EBEC at +0xC, 0x00C5ED50 at +0x0, 0x00C5ED48 at +0xC. Members are what xfer (0x004C9B44) transfers after Rva00362EC7::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0xC slot 0 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 2 -> 0x004C9A06 ?Rva004C9A06Get@@YAHXZ
//   +0x0 slot 11 -> 0x000A7801 ?get@Rva000A7801CmpBoolField@@QBE_NXZ
//   +0x0 slot 12 -> 0x004C9A82 ?clientUpdate@EvaAnnounceClientCreate@@UAEXXZ
//   +0x0 slot 13 -> 0x000CA841 ?vslot13@UpgradeSoundSelectorClientBehavior@@UAEPAXXZ
//   +0xC slot 1 -> 0x004C9A51 ?rva004C9A51@EvaAnnounceClientCreate@@UAEXXZ
//   +0xC slot 11 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
// Not established: interface tables at +0xC (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: ?xfer@ObjectModule@@QAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_EVA_ANNOUNCE_CLIENT_CREATE_H
#define BFME2_EVA_ANNOUNCE_CLIENT_CREATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Rva00362EC7.h"

class Thing;
class ModuleData;
class Xfer;

// Rva00362EC7 ends at 0xC; this class's members follow it.
typedef char EvaAnnounceClientCreate_base_size[(sizeof(Rva00362EC7) == 0xC) ? 1 : -1];

class EvaAnnounceClientCreate : public Rva00362EC7
{
public:
	EvaAnnounceClientCreate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0xC[0x4];
	UnsignedInt m_unk0x10;                         // +0x10; xfer UnsignedInt
	Bool m_unk0x14;                                // +0x14; xfer Bool
};
// SIZE 0x18
typedef char EvaAnnounceClientCreate_size[(sizeof(EvaAnnounceClientCreate) == 0x18) ? 1 : -1];
typedef char EvaAnnounceClientCreate_off_m_unk0x10[(offsetof(EvaAnnounceClientCreate, m_unk0x10) == 0x10) ? 1 : -1];
typedef char EvaAnnounceClientCreate_off_m_unk0x14[(offsetof(EvaAnnounceClientCreate, m_unk0x14) == 0x14) ? 1 : -1];

#endif
