// AnimatedParticleSysBoneClientUpdate.h: the AnimatedParticleSysBoneClientUpdate module. SIZE 0x10 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C8FE8 calls Rva00362EC7 first; it stores the
// tables 0x00C5EA50 at +0x0. Members are what xfer (0x004C9C7D) transfers after Rva00362EC7::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 2 -> 0x004C9008 ?Rva004C9008Get@@YAHXZ
//   +0x0 slot 12 -> 0x004C9053 ?clientUpdate@AnimatedParticleSysBoneClientUpdate@@UAEXXZ
//   +0x0 slot 16 -> 0x004C9593 ?loadPostProcess@SwayClientUpdate@@MAEXXZ
//   +0x0 slot 17 -> 0x004C90DA ?Rva004C90DAGet@@YAHXZ
//   +0x0 slot 27 -> 0x004C923B ?clientUpdate@SwayClientUpdate@@UAEXXZ
#pragma once
#ifndef BFME2_ANIMATED_PARTICLE_SYS_BONE_CLIENT_UPDATE_H
#define BFME2_ANIMATED_PARTICLE_SYS_BONE_CLIENT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Rva00362EC7.h"

class Thing;
class ModuleData;
class Xfer;

// Rva00362EC7 ends at 0xC; this class's members follow it.
typedef char AnimatedParticleSysBoneClientUpdate_base_size[(sizeof(Rva00362EC7) == 0xC) ? 1 : -1];

class AnimatedParticleSysBoneClientUpdate : public Rva00362EC7
{
public:
	AnimatedParticleSysBoneClientUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Int m_life;                                    // +0xC; constructor store; clientUpdate counts it up (Zero Hour name)
public:
	virtual void clientUpdate();	// override: ClientUpdateModule (Rva00362EC7) slot 12 (0x004C9053)
};
// SIZE 0x10
typedef char AnimatedParticleSysBoneClientUpdate_size[(sizeof(AnimatedParticleSysBoneClientUpdate) == 0x10) ? 1 : -1];
typedef char AnimatedParticleSysBoneClientUpdate_off_m_life[(offsetof(AnimatedParticleSysBoneClientUpdate, m_life) == 0xC) ? 1 : -1];

#endif
