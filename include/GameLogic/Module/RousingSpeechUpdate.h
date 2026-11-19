// RousingSpeechUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x004ACED2, 0x004ACED8);
// name, size, bases and virtual order are retail facts given below.
// RousingSpeechUpdate.h: the RousingSpeechUpdate module. SIZE 0x9C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004ACF76 calls SpecialAbilityUpdate first; it stores the
// tables 0x00C54FE8 at +0x0, 0x00C4D640 at +0xC, 0x00C54FD8 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004AD2AF) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004ACED2 ?name@RousingSpeechUpdate@@QBEPBDXZ
//   +0x0 slot 15 -> 0x004ACE11 ?startPreparation@RousingSpeechUpdate@@UAEXXZ
//   +0x0 slot 16 -> 0x00494E21 ?rva00494E21@Rva00494E21@@QAE_NXZ
//   +0x0 slot 17 -> 0x004AD399 ?triggerAbilityEffect@RousingSpeechUpdate@@UAEXXZ
//   +0x10 slot 0 -> 0x004AD019 ?update@RousingSpeechUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_ROUSING_SPEECH_UPDATE_H
#define BFME2_ROUSING_SPEECH_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char RousingSpeechUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class RousingSpeechUpdate : public SpecialAbilityUpdate
{
public:
	char const * name() const;	// 0x004ACED2
	RousingSpeechUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~RousingSpeechUpdate();

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x88[0x4];
	UnsignedInt m_unk0x8C;                         // +0x8C; xfer UnsignedInt
	Bool m_unk0x90;                                // +0x90; xfer Bool
	Real m_unk0x94;                                // +0x94; xfer Real
	Real m_unk0x98;                                // +0x98; xfer Real
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x9C
typedef char RousingSpeechUpdate_size[(sizeof(RousingSpeechUpdate) == 0x9C) ? 1 : -1];
typedef char RousingSpeechUpdate_off_m_unk0x8C[(offsetof(RousingSpeechUpdate, m_unk0x8C) == 0x8C) ? 1 : -1];
typedef char RousingSpeechUpdate_off_m_unk0x90[(offsetof(RousingSpeechUpdate, m_unk0x90) == 0x90) ? 1 : -1];
typedef char RousingSpeechUpdate_off_m_unk0x94[(offsetof(RousingSpeechUpdate, m_unk0x94) == 0x94) ? 1 : -1];
typedef char RousingSpeechUpdate_off_m_unk0x98[(offsetof(RousingSpeechUpdate, m_unk0x98) == 0x98) ? 1 : -1];

#endif
