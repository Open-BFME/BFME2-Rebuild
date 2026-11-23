// BeaconClientUpdate.h: the BeaconClientUpdate module. SIZE 0x14 (ModuleFactory operator new).
// Target facts: the constructor 0x004C9598 calls Rva00362EC7 first; it stores the
// tables 0x00C5EB30 at +0x0. Members are what xfer (0x004C95C9) transfers after Rva00362EC7::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 2 -> 0x004C95C3 ?Rva004C95C3Get@@YAHXZ
//   +0x0 slot 11 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 12 -> 0x004C98AF ?clientUpdate@BeaconClientUpdate@@UAEXXZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_BEACON_CLIENT_UPDATE_H
#define BFME2_BEACON_CLIENT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Rva00362EC7.h"

class Thing;
class ModuleData;
class Xfer;

// Rva00362EC7 ends at 0xC; this class's members follow it.
typedef char BeaconClientUpdate_base_size[(sizeof(Rva00362EC7) == 0xC) ? 1 : -1];

class BeaconClientUpdate : public Rva00362EC7
{
public:
	BeaconClientUpdate(Thing *thing, const ModuleData *moduleData);
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void clientUpdate();	// override: Rva00362EC7 slot 12 (0x004C98AF)
	void hideBeacon();
	virtual Bool vslot11() { return false; }	// slot 11: retail table 0x00C5EB30 has 0x0047A699 (`xor al,al`), not the base's 0x0050B5C6

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Int m_particleSystemID;                        // +0xC; xfer through XferParticleSystemID (0x0030600A), the constructor zeroes it
	UnsignedInt m_unk0x10;                         // +0x10; xfer UnsignedInt, the constructor stores the logic frame (TheGameLogic +0x40)
};
// SIZE 0x14
typedef char BeaconClientUpdate_size[(sizeof(BeaconClientUpdate) == 0x14) ? 1 : -1];
typedef char BeaconClientUpdate_off_m_unk0x10[(offsetof(BeaconClientUpdate, m_unk0x10) == 0x10) ? 1 : -1];

#endif
