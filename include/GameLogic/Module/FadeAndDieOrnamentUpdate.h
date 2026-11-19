// FadeAndDieOrnamentUpdate.h: the FadeAndDieOrnamentUpdate module. SIZE 0x50 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x003A439D calls UpdateModule first; it stores the
// tables 0x00C1AF60 at +0x0, 0x00BEFF90 at +0xC, 0x00C1AF54 at +0x10. Members are what xfer (0x003A4408) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004A40BB ?loadPostProcess@HijackerUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x003A433B ?name@Rva003A433BNamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x003A4473 ?update@Rva003A4322_B2@@QAEHXZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?, slot10 ?.
#pragma once
#ifndef BFME2_FADE_AND_DIE_ORNAMENT_UPDATE_H
#define BFME2_FADE_AND_DIE_ORNAMENT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char FadeAndDieOrnamentUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class FadeAndDieOrnamentUpdate : public UpdateModule
{
public:
	FadeAndDieOrnamentUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	ObjectID m_unk0x20;                            // +0x20; xfer ObjectID
	Real m_unk0x24;                                // +0x24; xfer Real
	UnsignedByte m_gap0x28[0x28];
};
// SIZE 0x50
typedef char FadeAndDieOrnamentUpdate_size[(sizeof(FadeAndDieOrnamentUpdate) == 0x50) ? 1 : -1];
typedef char FadeAndDieOrnamentUpdate_off_m_unk0x20[(offsetof(FadeAndDieOrnamentUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char FadeAndDieOrnamentUpdate_off_m_unk0x24[(offsetof(FadeAndDieOrnamentUpdate, m_unk0x24) == 0x24) ? 1 : -1];

#endif
