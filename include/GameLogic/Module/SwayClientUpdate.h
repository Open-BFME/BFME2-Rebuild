// SwayClientUpdate.h: the SwayClientUpdate module. SIZE 0x2C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C908B calls Rva00362EC7 first; it stores the
// tables 0x00C5EA8C at +0x0. Members are what xfer (0x004C9505) transfers after Rva00362EC7::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004C9593 ?loadPostProcess@SwayClientUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x004C90DA ?Rva004C90DAGet@@YAHXZ
//   +0x0 slot 12 -> 0x004C923B ?clientUpdate@SwayClientUpdate@@UAEXXZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_SWAY_CLIENT_UPDATE_H
#define BFME2_SWAY_CLIENT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Rva00362EC7.h"

class Thing;
class ModuleData;
class Xfer;

// Rva00362EC7 ends at 0xC; this class's members follow it.
typedef char SwayClientUpdate_base_size[(sizeof(Rva00362EC7) == 0xC) ? 1 : -1];

class SwayClientUpdate : public Rva00362EC7
{
public:
	SwayClientUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void clientUpdate();	// override: Rva00362EC7 slot 12 (0x004C923B)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();	// override: loadPostProcess slot 1 at +0x0 (0x004C9593)

public:
	Real m_unk0xC;                                 // +0xC; xfer Real
	Real m_unk0x10;                                // +0x10; xfer Real
	Real m_unk0x14;                                // +0x14; xfer Real
	Real m_unk0x18;                                // +0x18; xfer Real
	Real m_unk0x1C;                                // +0x1C; xfer Real
	Short m_unk0x20;                               // +0x20; xfer Short
	Bool m_unk0x22;                                // +0x22; xfer Bool
	Bool m_unk0x23;                                // +0x23; the constructor clears it, xfer does not transfer it
	Real m_unk0x24;                                // +0x24; xfer Real
	Real m_unk0x28;                                // +0x28; xfer Real
};
// SIZE 0x2C
typedef char SwayClientUpdate_size[(sizeof(SwayClientUpdate) == 0x2C) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0xC[(offsetof(SwayClientUpdate, m_unk0xC) == 0xC) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x10[(offsetof(SwayClientUpdate, m_unk0x10) == 0x10) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x14[(offsetof(SwayClientUpdate, m_unk0x14) == 0x14) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x18[(offsetof(SwayClientUpdate, m_unk0x18) == 0x18) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x1C[(offsetof(SwayClientUpdate, m_unk0x1C) == 0x1C) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x20[(offsetof(SwayClientUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x22[(offsetof(SwayClientUpdate, m_unk0x22) == 0x22) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x23[(offsetof(SwayClientUpdate, m_unk0x23) == 0x23) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x24[(offsetof(SwayClientUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char SwayClientUpdate_off_m_unk0x28[(offsetof(SwayClientUpdate, m_unk0x28) == 0x28) ? 1 : -1];

#endif
