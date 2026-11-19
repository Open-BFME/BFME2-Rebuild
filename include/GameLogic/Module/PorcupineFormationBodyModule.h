// PorcupineFormationBodyModule.h: the PorcupineFormationBodyModule module. SIZE 0x100 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C1FE6 calls ActiveBody first; it stores the
// tables 0x00C5C3C8 at +0x0, 0x00C5AD78 at +0xC, 0x00C5C318 at +0x10. Members are what xfer (0x004C20BA) transfers after ActiveBody::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004C202F ?rva004C202F@Rva004C202F@@QAEXXZ
//   +0x0 slot 2 -> 0x004C2010 ?Rva004C2010Get@@YAHXZ
//   +0x0 slot 23 -> 0x004C236A ?onDie@HeroDie@@UAEXPBVDamageInfo@@@Z
//   +0x0 slot 25 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 26 -> 0x004C2293 ?name@Rva004C2293Named@@QBEPBDXZ
//   +0x0 slot 29 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 33 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 34 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 0 -> 0x004C2230 ?attemptDamage@PorcupineFormationBodyModule@@UAEXPAVDamageInfo@@@Z
//   +0x10 slot 38 -> 0x004C2127 ?rva004C2127@PorcupineFormationBodyModule@@UAE_NPAVObject@@@Z
#pragma once
#ifndef BFME2_PORCUPINE_FORMATION_BODY_MODULE_H
#define BFME2_PORCUPINE_FORMATION_BODY_MODULE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBody.h"

class Thing;
class ModuleData;
class Xfer;

// ActiveBody ends at 0x100; this class's members follow it.
typedef char PorcupineFormationBodyModule_base_size[(sizeof(ActiveBody) == 0x100) ? 1 : -1];

class PorcupineFormationBodyModule : public ActiveBody
{
public:
	PorcupineFormationBodyModule(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual Bool rva004C2127(Object *object);
	virtual ~PorcupineFormationBodyModule();
	virtual void attemptDamage(DamageInfo *);	// override: attemptDamage slot 0 at +0x10 (0x004C2230)

protected:
	virtual void loadPostProcess() { ActiveBody::loadPostProcess(); }	// slot 1: retail 0x004C202F, a jmp to ActiveBody's (0x004BF40F), ICF-folded across the ActiveBody subclasses
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x100
typedef char PorcupineFormationBodyModule_size[(sizeof(PorcupineFormationBodyModule) == 0x100) ? 1 : -1];

#endif
