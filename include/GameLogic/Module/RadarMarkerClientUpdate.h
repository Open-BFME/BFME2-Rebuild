// RadarMarkerClientUpdate.h: the RadarMarkerClientUpdate module. SIZE 0x10 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C9BCD calls Rva00362EC7 first; it stores the
// tables 0x00C5EDB8 at +0x0. Members are what xfer (0x004C9C7D) transfers after Rva00362EC7::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 2 -> 0x004C9BED ?Rva004C9BEDGet@@YAHXZ
//   +0x0 slot 11 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 12 -> 0x004C9CF7 ?clientUpdate@RadarMarkerClientUpdate@@UAEXXZ
#pragma once
#ifndef BFME2_RADAR_MARKER_CLIENT_UPDATE_H
#define BFME2_RADAR_MARKER_CLIENT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Rva00362EC7.h"

class Thing;
class ModuleData;
class Xfer;

// Rva00362EC7 ends at 0xC; this class's members follow it.
typedef char RadarMarkerClientUpdate_base_size[(sizeof(Rva00362EC7) == 0xC) ? 1 : -1];

class RadarMarkerClientUpdate : public Rva00362EC7
{
public:
	RadarMarkerClientUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~RadarMarkerClientUpdate();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Int m_unk0xC;                                  // +0xC; constructor store
public:
	virtual void clientUpdate();	// override: ClientUpdateModule (Rva00362EC7) slot 12 (0x004C9CF7)
};
// SIZE 0x10
typedef char RadarMarkerClientUpdate_size[(sizeof(RadarMarkerClientUpdate) == 0x10) ? 1 : -1];
typedef char RadarMarkerClientUpdate_off_m_unk0xC[(offsetof(RadarMarkerClientUpdate, m_unk0xC) == 0xC) ? 1 : -1];

#endif
